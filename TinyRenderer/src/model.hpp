#pragma once

#include "geometry.hpp"
#include "tgaimage.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>


class Model{
private:
    std::vector<std::vector<Vec3i>> faces_;
    std::vector<Vec3f> verts_;
    std::vector<Vec3f> norms_;
    std::vector<Vec2f> uv_;
    TGAImage diffuse_map_;
    TGAImage normal_map_;
    TGAImage specular_map_;
    void load_texture(const char* filename,const char* suffix,TGAImage& img){
        std::string texfile(filename);
        auto dot=texfile.find_last_of(".");
        if(dot!=std::string::npos){
            texfile=texfile.substr(0,dot)+suffix;
            std::cerr<<"texture file "<<texfile<<" loaded "<<(img.read_tga_file(texfile.c_str())?"successfully":"unsuccessfully")<<std::endl;
            img.flip_vertically();
        }
    }
public:
    explicit Model(const char* filename){
        std::ifstream in(filename,std::ios::in);
        if(in.fail())return;
        std::string line;
        while(!in.eof()){
            std::getline(in,line);
            std::istringstream iss(line);
            char trash;
            //读取顶点数据
            if(!line.compare(0,2,"v ")){
                iss>>trash;
                Vec3f v;
                for(int i=0;i<3;++i){
                    iss>>v[i];
                }
                verts_.push_back(v);
            }
            //读取面数据
            else if(!line.compare(0,2,"f ")){
                std::vector<Vec3i> f;
                int idx,uv_idx,norm_idx;
                iss>>trash;
                while(iss>>idx>>trash>>uv_idx>>trash>>norm_idx){
                    //obj文件的索引从1开始，所以需要减一
                    f.emplace_back(idx-1,uv_idx-1,norm_idx-1);
                }
                faces_.push_back(f);
            }
            //读取纹理坐标
            else if(!line.compare(0,3,"vt ")){
                iss>>trash>>trash;
                Vec2f vt;
                for(int i=0;i<2;++i){
                    iss>>vt[i];
                }
                uv_.push_back(vt);
            }
            //读取法向量
            else if(!line.compare(0,3,"vn ")){
                iss>>trash>>trash;
                Vec3f vn;
                for(int i=0;i<3;++i){
                    iss>>vn[i];
                }
                norms_.push_back(vn);
            }
        }
        load_texture(filename,"_diffuse.tga",diffuse_map_);
        load_texture(filename,"_nm.tga",normal_map_);
        load_texture(filename,"_spec.tga",specular_map_);
    }
    ~Model()=default;
    [[nodiscard]]int nverts()const{
        return static_cast<int>(verts_.size());
    }
    [[nodiscard]]int nfaces()const{
        return static_cast<int>(faces_.size());
    }
    [[nodiscard]]int nuvs()const{
        return static_cast<int>(uv_.size());
    }
    [[nodiscard]]int nnorms()const{
        return static_cast<int>(norms_.size());
    }
    Vec3f vert(int i){
        return verts_[i];
    }
    std::vector<Vec3i> face(int idx){
        return faces_[idx];
    }
    Vec3f vert(int iface,int nvert){
        return verts_[faces_[iface][nvert][0]];
    }
    Vec2f uv(int iface,int nvert){
        return uv_[faces_[iface][nvert][1]];
    }
    Vec3f norm(int iface,int nvert){
        return normalize(norms_[faces_[iface][nvert][2]]);
    }
    TGAColor diffuse(Vec2f uvf){
        Vec2i uv(uvf[0]*diffuse_map_.get_width(),uvf[1]*diffuse_map_.get_height());
        return diffuse_map_.get(uv.x,uv.y);
    }
    Vec3f normal(Vec2f uvf){
        Vec2i uv(uvf[0]*normal_map_.get_width(),uvf[1]*normal_map_.get_height());
        TGAColor c=normal_map_.get(uv.x,uv.y);
        Vec3f res;
        for(int i=0;i<3;++i){
            //将法向量的范围从[0,255]映射到[-1,1]
            //RGB对应xyz，TGAColor存储的是BGR
            res[2-i]=static_cast<float>(c[i])/255.f*2.f-1.f;
        }
        return res;
    }
};