#pragma once
#include "tgaimage.h"
#include "geometry.hpp"
#include "model.hpp"

extern Mat44f Model_;
extern Mat44f View;
extern Mat44f Projection;
extern Mat44f Viewport;
extern Vec3f light_dir;
extern const float depth;

struct IShader{
    static TGAColor sample2D(const TGAImage& image,const Vec2f uvf){
        return image.get(static_cast<int>(uvf[0]*image.get_width()),static_cast<int>(uvf[1]*image.get_height()));
    }
    virtual ~IShader()=default;
    virtual void vertex(const int iface,const int nvert,Vec4f& gl_Postion)=0;
    virtual bool fragment(const Vec3f bar,TGAColor& color)=0;
};

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
/*
struct DepthShader:IShader{
    Model* model;
    Mat44f uniform_mvp;
    Mat44f uniform_mv;
    Mat43f varying_posV;

    DepthShader(Model* model):model(model){
        uniform_mv=View*Model_;
        uniform_mvp=Projection*View*Model_;
    }

    void vertex(const int iface,const int nvert,Vec4f& gl_Position)override{
        Vec4f gl_vertex=embed<4>(model->vert(iface,nvert));
        varying_posV.set_col(nvert,uniform_mv*gl_vertex);
        gl_Position=uniform_mvp*gl_vertex;
    }

    bool fragment(const Vec3f bar,TGAColor& gl_FragColor)override{
        Vec3f z={varying_posV[0][2],varying_posV[1][2],varying_posV[2][2]};
        float z_bar=z*bar;
        gl_FragColor=TGAColor(255,255,255)*(z_bar);
        return false;
    }

};*/