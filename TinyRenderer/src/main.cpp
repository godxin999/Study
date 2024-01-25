#include "model.hpp"
#include "my_gl.hpp"
#include <algorithm>
#include <random>


Model* model = nullptr;
float* zbuffer = nullptr;
constexpr int height=2000;
constexpr int width=2000;
//光源的方向指向(0,0,0)
Vec3f light_dir={1,1,1};
Vec3f eye{1,1,3};
Vec3f center{0,0,0};
Vec3f up{0,1,0};
/*
struct FlatShader:IShader{
    float intensity;
    virtual Vec4f vertex(int iface,int nvert){
        if(nvert==0){
            Vec3f vert[3]={
                model->vert(iface,0),
                model->vert(iface,1),
                model->vert(iface,2)
            };
            Vec3f n=cross(vert[2]-vert[0],vert[1]-vert[0]);
            n.normalize();
            intensity=n*light_dir;
        }
        Vec4f gl_vertex=embed<4>(model->vert(iface,nvert));
        gl_vertex=Viewport*Projection*View*Model_*gl_vertex;
        return gl_vertex;
    }
    virtual bool fragment(Vec3f bar,TGAColor& color){
        color=TGAColor(255,255,255)*intensity;
        return false;
    }
};
*/
struct GouraudShader:IShader{
    mat<2,3,float> varying_uv;
    Mat44f uniform_mvp;
    Mat44f uniform_mvp_inv_transpose;
    virtual Vec4f vertex(int iface,int nvert){
        varying_uv.set_col(nvert,model->uv(iface,nvert));
        Vec4f gl_vertex=embed<4>(model->vert(iface,nvert));
        return Viewport*Projection*View*Model_*gl_vertex;
    }
    virtual bool fragment(Vec3f bar,TGAColor& color){
        Vec2f uv=varying_uv*bar;
        Vec3f n=proj<3>(uniform_mvp_inv_transpose*embed<4>(model->normal(uv),0.f)).normalize();
        Vec3f l=proj<3>(uniform_mvp*embed<4>(light_dir,0.f)).normalize();
        float intensity=std::max(0.f,n*l);
        color=model->diffuse(uv)*intensity;
        return false;
    }
};

void render(TGAImage &image){
    Model_=model_matrix();
    View=view_matrix(eye,center,up);
    Projection=projection_matrix((eye-center).norm());
    Viewport=viewport_matrix(width/8,height/8,width*3/4,height*3/4);
    light_dir.normalize();
    GouraudShader shader;
    shader.uniform_mvp=Projection*View*Model_;
    shader.uniform_mvp_inv_transpose=(Projection*View*Model_).invert_transpose();
    for(int i=0;i<model->nfaces();++i){
        Vec4f screen[3];
        for(int j=0;j<3;++j){
            screen[j]=shader.vertex(i,j);
        }
        triangle(screen,shader,image,zbuffer);
    }
}

int main() {
    model=new Model("../../../../assets/african_head.obj");
    zbuffer=new float[width*height];
    std::fill_n(zbuffer,width*height,min_float);

    TGAImage image(width, height, TGAImage::RGB);

    render(image);
    image.flip_vertically();
	image.write_tga_file("output.tga");

    delete model;
    delete []zbuffer;
    return 0;
}
