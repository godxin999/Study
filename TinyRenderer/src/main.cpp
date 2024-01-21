#include "model.hpp"
#include "common.hpp"

Model* model = nullptr;
float *zbuffer = nullptr;

Vec3f light_dir{0,0,-1};
Vec3f camera{0,0,3};

Vec3f m2v(mat<4,1,float> m){
    Vec3f v;
    v.x=m[0][0]/m[3][0];
    v.y=m[1][0]/m[3][0];
    v.z=m[2][0]/m[3][0];
    return v;
}

mat<4,1,float> v2m(Vec3f v){
    mat<4,1,float> m;
    m[0][0]=v.x;
    m[1][0]=v.y;
    m[2][0]=v.z;
    m[3][0]=1.;
    return m;
}

Mat44f viewport(int x,int y,int w,int h){
    Mat44f m=Mat44f::identity();
    m[0][3]=x+w/2.f;
    m[1][3]=y+h/2.f;
    m[2][3]=depth/2.f;
    m[0][0]=w/2.f;
    m[1][1]=h/2.f;
    m[2][2]=depth/2.f;
    return m;
}

void triangle(Vec3f *pts,Vec2i *uvs,TGAImage &image,float* zbuffer,float intensity){
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
                Vec2i uv=uvs[0]*bc.x+uvs[1]*bc.y+uvs[2]*bc.z;
                //计算纹理颜色
                TGAColor color=model->diffuse(uv);
                image.set(p.x,p.y,TGAColor(color.r*intensity,color.g*intensity,color.b*intensity));
            }
        }
    }
}

void render(TGAImage &image){
    Mat44f Projection=Mat44f::identity();
    Mat44f Viewport=viewport(width/8,height/8,width*3/4,height*3/4);
    Projection[3][2]=-1.f/camera.z;
    for(int i=0;i<model->nfaces();++i){
        const auto face=model->face(i);
        Vec3f screen[3];
        Vec3f world[3];
        for(int j=0;j<3;++j){
            screen[j]=m2v(Viewport*Projection*v2m(model->vert(face[j].x)));
            world[j]=model->vert(face[j].x);
        }
        //计算三角形的法向量
        Vec3f n=cross((world[2]-world[0]),(world[1]-world[0]));
        //法向量归一化
        n.normalize();
        //计算光照强度
        float intensity=n*light_dir;
        //如果光照强度大于0，说明三角形面朝向光源，需要绘制
        if(intensity>0){
            Vec2i uvs[3];
            for(int j=0;j<3;++j){
                uvs[j]=model->uv_diffuse(i, j);
            }
            triangle(screen,uvs,image,zbuffer,intensity);
        }
    }
}

int main() {
    model=new Model("../../../../assets/african_head.obj");
    zbuffer=new float[width*height];
    std::fill_n(zbuffer,width*height,min_float);

    TGAImage image(width, height, TGAImage::RGB);
    render(image);
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("head_diffuse_proj.tga");
    delete model;
    delete[] zbuffer;
    return 0;
}
