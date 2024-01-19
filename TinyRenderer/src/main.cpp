#include <cmath>
#include <iostream>
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
/*
void get_model(){
    model=new Model("../../../../assets/african_head.obj");
	TGAImage image(width, height, TGAImage::RGB);
    for(int i=0;i<model->nfaces();++i){
        const auto &face=model->face(i);
        //每两个顶点间画一条线
        for(int j=0;j<3;++j){
            const auto &v0=model->vert(face[j]);
            const auto &v1=model->vert(face[(j+1)%3]);
            //将[-1,1]的坐标转换为[0,width]和[0,height]的坐标
            int x0=(v0.x+1.)*width/2.;
            int y0=(v0.y+1.)*height/2.;
            int x1=(v1.x+1.)*width/2.;
            int y1=(v1.y+1.)*height/2.;
            line(x0,y0,x1,y1,image,white);
        }
    }
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("head.tga");
}
*/
/*
void my_triangle(Vec2i t0,Vec2i t1,Vec2i t2,TGAImage &image,const TGAColor &color){
    line(t0,t1,image,color);
    line(t1,t2,image,color);
    line(t2,t0,image,color);
    if(t2.y>t1.y)std::swap(t2,t1);
    if(t1.y>t0.y)std::swap(t1,t0);
    if(t2.y>t1.y)std::swap(t2,t1);
    int total_height=t0.y-t2.y;
    int seg_height=t1.y-t2.y;
    for(int i=t2.y;i<=t0.y;++i){
        float alpha=(i-t2.y)/static_cast<float>(total_height);
        Vec2i A(std::lerp(t2.x,t0.x,alpha),i);
        Vec2i B;
        float beta;
        if(i<=t1.y){
            beta=1+(i-t1.y)/static_cast<float>(seg_height);
            B.x=std::lerp(t2.x,t1.x,beta);
            B.y=i;
        }
        else{
            float beta=(i-t1.y)/static_cast<float>(total_height-seg_height);
            B.x=std::lerp(t1.x,t0.x,beta);
            B.y=i;
        }
        line(A,B,image,color);
    }
}
*/
void triangle(Vec2i t0,Vec2i t1,Vec2i t2,TGAImage &image,const TGAColor &color){
    if(t2.y>t1.y)std::swap(t2,t1);
    if(t1.y>t0.y)std::swap(t1,t0);
    if(t2.y>t1.y)std::swap(t2,t1);
    int total_height=t0.y-t2.y;
    for(int i=t2.y;i<=t0.y;++i){
        bool second_half=(i>=t1.y||t1.y==t2.y);
        int seg_height=second_half?t0.y-t1.y:t1.y-t2.y;
        float alpha=(i-t2.y)/static_cast<float>(total_height);
        float beta=(second_half?i-t1.y:i-t2.y)/static_cast<float>(seg_height);
        Vec2i A(std::lerp(t2.x,t0.x,alpha),i);
        Vec2i B(second_half?std::lerp(t1.x,t0.x,beta):std::lerp(t2.x,t1.x,beta),i);
        if(A.x>B.x)std::swap(A,B);
        for(int j=A.x;j<=B.x;++j){
            image.set(j,i,color);
        }
    }
}
int main() {
    TGAImage image(width, height, TGAImage::RGB);

    Vec2i t0[3] = {Vec2i(100, 700),   Vec2i(500, 1600),  Vec2i(700, 800)};
    Vec2i t1[3] = {Vec2i(1800, 500),  Vec2i(1500, 10),   Vec2i(700, 1800)};
    Vec2i t2[3] = {Vec2i(1800, 1500), Vec2i(1200, 1600), Vec2i(1300, 1800)};
    triangle(t0[0], t0[1], t0[2], image, red);
    triangle(t1[0], t1[1], t1[2], image, white);
    triangle(t2[0], t2[1], t2[2], image, green);

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}