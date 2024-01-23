#pragma once

#include "tgaimage.h"
#include "geometry.hpp"
#include <algorithm>

inline const TGAColor white = TGAColor(255, 255, 255, 255);
inline const TGAColor red   = TGAColor(255, 0,   0,   255);
inline const TGAColor green = TGAColor(0,   255, 0,   255);
inline constexpr int height=2000;
inline constexpr int width=2000;
inline constexpr int depth=255;

//Bresenham's line algorithm
void line(Vec2i p0,Vec2i p1,TGAImage& image,const TGAColor& color){
    //判断直线斜率是否大于1或小于-1，通过交换x、y轴防止斜率过大导致误差过大
    bool steep=false;
    if(std::abs(p0.x-p1.x)<std::abs(p0.y-p1.y)){
        std::swap(p0.x,p0.y);
        std::swap(p1.x,p1.y);
        steep=true;
    }
    //保证x0<x1
    if(p0.x>p1.x){
        std::swap(p0,p1);
    }
    //以像素为单位模拟插值过程(以2倍的dx作为误差的判断标准)
    int dx=p1.x-p0.x;
    int dy=p1.y-p0.y;
    int derror=std::abs(dy)*2;//除以2dx即为斜率
    int error=0;
    int y=p0.y;
    for(int x=p0.x;x<=p1.x;++x){
        if(steep){
            image.set(y,x,color);//交换x、y轴
        }
        else{
            image.set(x,y,color);
        }
        error+=derror;
        if(error>dx){//除以2dx即为>0.5，即像素误差大于0.5，此时必须移动像素的y坐标
            y+=(p1.y>p0.y?1:-1);//根据斜率正负决定y+1或y-1
            error-=dx*2;//除以2dx即为-1，即像素误差减一
        }
    }
}


//barycentric coordinates calculation
Vec3f barycentric(Vec3f A,Vec3f B,Vec3f C,Vec3f P){
    //P=(1-u-v)A+uB+vC
    //PA+uAB+vAC=0
    //(v,u,1)(AC,AB,PA)^T=0
    //(v,u,1)(AC_x,AB_x,PA_x)^T=0
    //(v,u,1)(AC_y,AB_y,PA_y)^T=0
    //k(v,u,1)=(AC_x,AB_x,PA_x)×(AC_y,AB_y,PA_y)
    //以x分量和y分量的外积来计算重心坐标
    Vec3f s[2];
    for(int i=2;i>=0;--i){
        s[i][0]=C[i]-A[i];
        s[i][1]=B[i]-A[i];
        s[i][2]=A[i]-P[i];
    }
    auto u=cross(s[0],s[1]);
    //因为pts和p的坐标都是整数，所以u的坐标也是整数，所以u的坐标的绝对值小于1时表示三角形退化为线段或点
    if(std::abs(u[2])>1e-2)return {1.f-(u.x+u.y)/u.z,u.y/u.z,u.x/u.z};
    return {-1,1,1};
}

//获取视口变换矩阵
Mat44f viewport_matrix(int x,int y,int w,int h){
    //将[-1,1]*[-1,1]*[-1,1]映射到[x,x+w]*[y,y+h]*[0,depth]
    Mat44f m=Mat44f::identity();
    m[0][3]=x+w/2.f;
    m[1][3]=y+h/2.f;
    m[2][3]=depth/2.f;
    m[0][0]=w/2.f;
    m[1][1]=h/2.f;
    m[2][2]=depth/2.f;
    return m;
}

//获取透视投影矩阵
//dist为视点到投影平面的距离
Mat44f projection_matrix(float dist){
    //以标准坐标轴为例，相机位于(0,0,c)，被投影点坐标为(x,y,z)，投影平面为z=0，投影后点坐标为(x',y',0)
    //根据相似三角形，x/x'=(c-z)/c，y/y'=(c-z)/c，故x'=x/(1-z/c)，y'=y/(1-z/c)
    //1 0 0 0  * (x,y,z,1)^T=(x,y,z,rz+1)^T
    //0 1 0 0
    //0 0 1 0
    //0 0 r 1
    //(x,y,z,rz+1)->(x/(rz+1),y/(rz+1),z,rz+1)
    //所以退出r=-1/c，即-1/dist
    Mat44f m=Mat44f::identity();
    m[3][2]=-1.f/dist;
    return m;
}

//获取视图变换矩阵
Mat44f view_matrix(Vec3f eye,Vec3f center,Vec3f up){
    //首先使用lookat方式计算出相机坐标系的基
    //右手系所以是eye-center，左手系是center-eye，根据z轴的正方向来判断
    auto f=(eye-center).normalize();
    auto r=cross(up,f).normalize();
    //因为u不一定和up正交，所以需要重新计算
    auto u=cross(f,r).normalize();
    //根据相机坐标系的基计算出变换矩阵
    Mat44f v=Mat44f::identity();
    //从世界坐标系变换到相机坐标系需要一个旋转变换(基变换)和一个平移变换
    //视图矩阵表示的是上述变换的逆变换，即
    //V=(TR)^(-1)=R^(-1)T^(-1)
    //R为基变换矩阵即(r,u,f,1)
    //T为平移变换矩阵即
    //1,0,0,eye.x
    //0,1,0,eye.y
    //0,0,1,eye.z
    //0,0,0,1
    //因为旋转矩阵是正交矩阵，所以R^(-1)=R^T
    //T^(-1)即将平移向量取反
    //所以V=R^(-1)T^(-1)=
    //r.x,r.y,r.z,-r·eye
    //u.x,u.y,u.z,-u·eye
    //f.x,f.y,f.z,-f·eye
    //0,0,0,1
    v[0]=embed<4>(r,-r*eye);
    v[1]=embed<4>(u,-u*eye);
    v[2]=embed<4>(f,-f*eye);
    return v;
}

//获取模型变换矩阵
Mat44f model_matrix(){
    Mat44f m=Mat44f::identity();
    return m;
}