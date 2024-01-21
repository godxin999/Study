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

