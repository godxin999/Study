#include <iostream>
#include <limits>
#include <random>


#include "model.hpp"
#include "common.hpp"

Model* model = nullptr;
float *zbuffer = nullptr;

Vec3f light_dir{0,0,-1};

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
