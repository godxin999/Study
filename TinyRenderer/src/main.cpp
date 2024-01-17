#include "tgaimage.h"

#include "geometry.hpp"
#include "model.hpp"


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
Model* model = nullptr;
const int width=1000;
const int height=1000;

//Bresenham's line algorithm
void line(int x0,int y0,int x1,int y1,TGAImage& image,const TGAColor& color){
    //判断直线斜率是否大于1或小于-1，通过交换x、y轴防止斜率过大导致误差过大
    bool steep=false;
    if(std::abs(x0-x1)<std::abs(y0-y1)){
        std::swap(x0,y0);
        std::swap(x1,y1);
        steep=true;
    }
    //保证x0<x1
    if(x0>x1){
        std::swap(x0,x1);
        std::swap(y0,y1);
    }
    //以像素为单位模拟插值过程(以2倍的dx作为误差的判断标准)
    int dx=x1-x0;
    int dy=y1-y0;
    int derror=std::abs(dy)*2;//除以2dx即为斜率
    int error=0;
    int y=y0;
    for(int x=x0;x<=x1;++x){
        if(steep){
            image.set(y,x,color);//交换x、y轴
        }
        else{
            image.set(x,y,color);
        }
        error+=derror;
        if(error>dx){//除以2dx即为>0.5，即像素误差大于0.5，此时必须移动像素的y坐标
            y+=(y1>y0?1:-1);//根据斜率正负决定y+1或y-1
            error-=dx*2;//除以2dx即为-1，即像素误差减一
        }
    }
}

int main() {
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
	return 0;
}

