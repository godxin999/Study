#include <cmath>
#include <iostream>
#include <random>
#include <algorithm>

#include "tgaimage.h"

#include "geometry.hpp"
#include "model.hpp"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);
Model* model = nullptr;
const int width=2000;
const int height=2000;
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
Vec3f barycentric(Vec2i *pts,Vec2i p){
    //P=(1-u-v)A+uB+vC
    //PA+uAB+vAC=0
    //(v,u,1)(AC,AB,PA)^T=0
    //(v,u,1)(AC_x,AB_x,PA_x)^T=0
    //(v,u,1)(AC_y,AB_y,PA_y)^T=0
    //(v,u,1)=(AC_x,AB_x,PA_x)×(AC_y,AB_y,PA_y)
    //以x分量和y分量的外积来计算重心坐标
    auto u=Vec3f(pts[2].x-pts[0].x,pts[1].x-pts[0].x,pts[0].x-p.x)^Vec3f(pts[2].y-pts[0].y,pts[1].y-pts[0].y,pts[0].y-p.y);
    //因为pts和p的坐标都是整数，所以u的坐标也是整数，所以u的坐标的绝对值小于1时表示三角形退化为线段或点
    if(std::abs(u.z)<1)return {-1,1,1};
    return {1.f-(u.x+u.y)/u.z,u.y/u.z,u.x/u.z};
}
void triangle(Vec2i *pts,TGAImage &image,const TGAColor &color){
    Vec2i bboxmin(image.get_width()-1,image.get_height()-1);
    Vec2i bboxmax(0,0);
    Vec2i clamp(image.get_width()-1,image.get_height()-1);
    //求出三角形的包围盒
    for(int i=0;i<3;++i){
        bboxmin.x=std::clamp(pts[i].x,0,bboxmin.x);
        bboxmin.y=std::clamp(pts[i].y,0,bboxmin.y);
        bboxmax.x=std::clamp(pts[i].x,bboxmax.x,clamp.x);
        bboxmax.y=std::clamp(pts[i].y,bboxmax.y,clamp.y);
    }
    Vec2i p;
    for(p.x=bboxmin.x;p.x<=bboxmax.x;++p.x){
        for(p.y=bboxmin.y;p.y<=bboxmax.y;++p.y){
            //对包围盒中的每个点，求其重心坐标
            auto bc=barycentric(pts, p);
            //如果重心坐标中有负数，说明点在三角形外
            if(bc.x<0||bc.y<0||bc.z<0)continue;
            image.set(p.x,p.y,color);
        }
    }
}

void get_model(TGAImage &image){
    model=new Model("../../../../assets/african_head.obj");
    //std::random_device rd;
	//std::mt19937 rng(rd());
	//std::uniform_int_distribution<int> uni(0,255);
    for(int i=0;i<model->nfaces();++i){
        const auto &face=model->face(i);
        Vec2i screen_coords[3];
        Vec3f world_coords[3];
        for(int j=0;j<3;++j){
            const auto &v=model->vert(face[j]);
            //将[-1,1]的坐标转换为[0,width]和[0,height]的坐标
            screen_coords[j]=Vec2i((v.x+1.)*width/2.,(v.y+1.)*height/2.);
            world_coords[j]=v;
        }
        //获取三角形的法向量
        Vec3f n=(world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
        //将法向量转换为单位向量
        n.normalize();
        //计算光强
        float intensity=n*light_dir;
        //对光强小于0的三角形进行剔除
        if(intensity>0){
            triangle(screen_coords,image,TGAColor(intensity*255,intensity*255,intensity*255,255));
        }
    }
}

int main() {
    TGAImage image(width, height, TGAImage::RGB);
    /*
    Vec2i t0[3] = {Vec2i(100, 700),   Vec2i(500, 1600),  Vec2i(700, 800)};
    Vec2i t1[3] = {Vec2i(1800, 500),  Vec2i(1500, 10),   Vec2i(700, 1800)};
    Vec2i t2[3] = {Vec2i(1800, 1500), Vec2i(1200, 1600), Vec2i(1300, 1800)};
    triangle(t0, image, red);
    triangle(t1, image, white);
    triangle(t2, image, green);
    */
    get_model(image);
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("head.tga");
	return 0;
}