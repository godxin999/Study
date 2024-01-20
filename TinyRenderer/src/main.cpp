#include <cmath>
#include <iostream>
#include <limits>
#include <random>
#include <algorithm>

#include "tgaimage.h"

#include "geometry.hpp"
#include "model.hpp"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);
Model* model = nullptr;
float *zbuffer = nullptr;
constexpr int width=2000;
constexpr int height=2000;
Vec3f light_dir{0,0,-1};

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
    //(v,u,1)=(AC_x,AB_x,PA_x)×(AC_y,AB_y,PA_y)
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

Vec3f world2screen(Vec3f v) {
    return Vec3f(static_cast<int>((v.x+1.)*width/2.+.5), static_cast<int>((v.y+1.)*height/2.+.5), v.z);
}

void triangle(Vec3f *pts,Vec2f *uvs,float* zbuffer,TGAImage &image,TGAImage &texture,float intensity){
    Vec2i bboxmin(image.get_width()-1,image.get_height()-1);
    Vec2i bboxmax(0,0);
    Vec2i clamp(image.get_width()-1,image.get_height()-1);
    //求出三角形的包围盒
    for(int i=0;i<3;++i){
        bboxmin.x=std::clamp(static_cast<int>(pts[i].x),0,bboxmin.x);
        bboxmin.y=std::clamp(static_cast<int>(pts[i].y),0,bboxmin.y);
        bboxmax.x=std::clamp(static_cast<int>(pts[i].x),bboxmax.x,clamp.x);
        bboxmax.y=std::clamp(static_cast<int>(pts[i].y),bboxmax.y,clamp.y);
    }
    Vec3f p;
    for(p.x=bboxmin.x;p.x<=bboxmax.x;++p.x){
        for(p.y=bboxmin.y;p.y<=bboxmax.y;++p.y){
            //对包围盒中的每个点，求其重心坐标
            auto bc=barycentric(pts[0],pts[1],pts[2], p);
            //如果重心坐标中有负数，说明点在三角形外
            if(bc.x<0||bc.y<0||bc.z<0)continue;
            //如果点在三角形内，计算其深度值
            p.z=0;
            for(int i=0;i<3;++i){
                p.z+=pts[i][2]*bc[i];
            }
            //如果深度值大于zbuffer中的值，说明该点在三角形前面，需要绘制
            if(zbuffer[static_cast<int>(p.x+p.y*width)]<p.z){
                zbuffer[static_cast<int>(p.x+p.y*width)]=p.z;
                //计算纹理坐标
                Vec2f uv=uvs[0]*bc.x+uvs[1]*bc.y+uvs[2]*bc.z;
                //计算纹理颜色
                TGAColor color=texture.get(static_cast<int>(uv.x*texture.get_width()),static_cast<int>(uv.y*texture.get_height()));
                color.r*=intensity;
                color.g*=intensity;
                color.b*=intensity;
                image.set(p.x,p.y,color);
            }
        }
    }
}

void render(TGAImage &image,TGAImage &texture){
    for(int i=0;i<width*height;++i){
        zbuffer[i]=-std::numeric_limits<float>::max();
    }
    for(int i=0;i<model->nfaces();++i){
        const auto &face=model->face(i);
        Vec3f screen[3];
        Vec3f world[3];
        Vec2f uv[3];
        for(int j=0;j<3;++j){
            screen[j]=world2screen(model->vert(face[j].x));
            world[j]=model->vert(face[j].x);
            uv[j]=model->uv(face[j].y);
        }
        //计算三角形的法向量
        Vec3f n=cross((world[2]-world[0]),(world[1]-world[0]));
        //法向量归一化
        n.normalize();
        //计算光照强度
        float intensity=n*light_dir;
        //如果光照强度大于0，说明三角形面朝向光源，需要绘制
        if(intensity>0){
            triangle(screen,uv,zbuffer,image,texture,intensity);
        }
    }
}

int main() {
    model=new Model("../../../../assets/african_head.obj");
    zbuffer=new float[width*height];

    TGAImage image(width, height, TGAImage::RGB);
    TGAImage texture;
    texture.read_tga_file("../../../../assets/african_head_diffuse.tga");
    texture.flip_vertically();
    
    render(image,texture);

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("head_diffuse.tga");
    
    delete model;
    delete[] zbuffer;
	return 0;
}
