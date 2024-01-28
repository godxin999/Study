#include "model.hpp"
#include "my_gl.hpp"
#include <algorithm>
#include <random>
#include <vector>


Model* model = nullptr;
constexpr int height=2000;
constexpr int width=2000;
std::vector<float> zbuffer(width*height,min_float);
//光源的方向指向(0,0,0)
Vec3f light_dir={1,1,1};
Vec3f eye{1,1,3};
Vec3f center{0,0,0};
Vec3f up{0,1,0};


struct PhongShader:IShader{
    Model* model;
    Mat44f uniform_mv;
    Mat44f uniform_mv_int;
    Mat44f uniform_mvp;
    PhongShader(Model* model):model(model){
        uniform_lightV=proj<3>(View*Model_*embed<4>(light_dir,0.f));
        uniform_mv=View*Model_;
        uniform_mv_int=uniform_mv.invert_transpose();
        uniform_mvp=Projection*View*Model_;
    }

    Vec3f uniform_lightV;//光源在视图空间下的方向
    Mat23f varying_uv;//顶点uv坐标
    Mat33f varying_normV;//顶点法向量在视图空间下的坐标
    Mat43f varying_posV;//顶点在视图空间下的坐标

    Vec3f get_normal(const Vec3f bar){
        //插值uv坐标
        auto uv=varying_uv*bar;
        //插值法向量
        auto bn=(varying_normV*bar).normalize();
        //计算TBN矩阵
        Mat32f e;
        e.set_col(0,proj<3>(varying_posV.col(1)-varying_posV.col(0)));
        e.set_col(1,proj<3>(varying_posV.col(2)-varying_posV.col(0)));
        Mat22f delta_uv;
        delta_uv.set_col(0, varying_uv.col(1) - varying_uv.col(0));
        delta_uv.set_col(1, varying_uv.col(2) - varying_uv.col(0));
        Mat32f TB=e*delta_uv.invert();
        //以法向量为第一个向量，对切线进行施密特正交化
        Mat33f TBN;
        TBN.set_col(2, bn);
        TBN.set_col(0,(TB.col(0)-bn*(bn*TB.col(0))).normalize());
        //使用外积计算副切线
        TBN.set_col(1,cross(TBN.col(2),TBN.col(0)));
        //将法向量从切线空间转换到视图空间
        return (TBN*model->normal(uv)).normalize();
    }

    void vertex(const int iface,const int nvert,Vec4f& gl_Position)override{
        varying_uv.set_col(nvert,model->uv(iface,nvert));
        //使用逆转置矩阵变换法线
        varying_normV.set_col(nvert,proj<3>(uniform_mv_int*embed<4>(model->normal(iface,nvert),0.f)));
        Vec4f gl_vertex=embed<4>(model->vert(iface,nvert));
        varying_posV.set_col(nvert,uniform_mv*gl_vertex);
        gl_Position=uniform_mvp*gl_vertex;
    }

    bool fragment(const Vec3f bar,TGAColor& gl_FragColor)override{
        auto uv=varying_uv*bar;
        //从切线空间法线贴图中获取法线
        auto n=get_normal(bar);
        //从法线贴图中获取法线
        //auto n=model->normal(uv);
        float diff=std::max(0.f,n*uniform_lightV);
        auto r=(n*(n*uniform_lightV*2.f)-uniform_lightV).normalize();
        float spec=std::pow(std::max(r.z,0.0f),5+sample2D(model->specular(),uv)[0]);
        TGAColor c=sample2D(model->diffuse(),uv);
        for(int i=0;i<3;++i){
            gl_FragColor[i]=std::min(10.f+c[i]*(diff+.5f*spec),255.f);
        }
        return false;
    }

};

void render(TGAImage &image){
    Model_=model_matrix();
    View=view_matrix(eye,center,up);
    Projection=projection_matrix((eye-center).norm());
    Viewport=viewport_matrix(width/8,height/8,width*3/4,height*3/4);
    PhongShader shader(model);
    
    for(int i=0;i<model->nfaces();++i){
        Vec4f clip_vert[3];
        for(int j=0;j<3;++j){
            shader.vertex(i,j,clip_vert[j]);
        }
        triangle(clip_vert,shader,image,zbuffer);
    }
}

int main() {
    //model=new Model("../../../../assets/african_head/african_head.obj");
    model=new Model("../../../../assets/diablo3_pose/diablo3_pose.obj");
    TGAImage image(width, height, TGAImage::RGB);

    render(image);
    image.flip_vertically();
	image.write_tga_file("output.tga");

    delete model;
    return 0;
}
