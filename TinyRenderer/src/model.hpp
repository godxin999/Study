#pragma once

#include "geometry.hpp"
#include "tgaimage.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>


class Model{
private:
    std::vector<Vec3f> verts;
    std::vector<Vec2f> tex_coord;
    std::vector<Vec3f> norms;
    TGAImage diffuse_map;
    TGAImage normal_map;
    TGAImage specular_map;
    std::vector<int> vert_idx;
    std::vector<int> tex_idx;
    std::vector<int> norm_idx;
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
                verts.push_back(v);
            }
            //读取面数据
            else if(!line.compare(0,2,"f ")){
                int v, t, n;
                iss >> trash;
                int cnt = 0;
                while(iss >> v >> trash >> t >> trash >> n){
                    //obj文件的索引从1开始，所以需要减一
                    vert_idx.push_back(v-1);
                    tex_idx.push_back(t-1);
                    norm_idx.push_back(n-1);                    
                    ++cnt;
                }
                assert(cnt == 3 && "Model file must be triangulated");
            }
            //读取纹理坐标
            else if(!line.compare(0,3,"vt ")){
                iss>>trash>>trash;
                Vec2f vt;
                for(int i=0;i<2;++i){
                    iss>>vt[i];
                }
                tex_coord.emplace_back(vt[0],vt[1]);
            }
            //读取法向量
            else if(!line.compare(0,3,"vn ")){
                iss>>trash>>trash;
                Vec3f vn;
                for(int i=0;i<3;++i){
                    iss>>vn[i];
                }
                norms.push_back(vn.normalize());
            }
        }

        load_texture(filename,"_diffuse.tga",diffuse_map);
        //load_texture("../../../../assets/grid.obj",".tga",diffuse_map);
        //load_texture(filename,"_nm.tga",normal_map);
        load_texture(filename,"_nm_tangent.tga",normal_map);
        load_texture(filename,"_spec.tga",specular_map);
    }
    ~Model()=default;
    [[nodiscard]]int nverts()const{
        return static_cast<int>(verts.size());
    }
    [[nodiscard]]int nfaces()const{
        return static_cast<int>(vert_idx.size()/3);
    }
    [[nodiscard]]Vec3f vert(const int i)const{
        return verts[i];
    }
    [[nodiscard]]Vec3f vert(const int iface,const int nvert)const{
        return verts[vert_idx[iface*3+nvert]];
    }
    [[nodiscard]]Vec2f uv(const int iface,const int nvert)const{
        return tex_coord[tex_idx[iface*3+nvert]];
    }
    [[nodiscard]]Vec3f normal(const int iface,const int nvert)const{
        return normalize(norms[norm_idx[iface*3+nvert]]);
    }

    Vec3f normal(Vec2f uvf){
        Vec2i uv(uvf[0]*normal_map.get_width(),uvf[1]*normal_map.get_height());
        TGAColor c=normal_map.get(uv.x,uv.y);
        Vec3f res;
        for(int i=0;i<3;++i){
            //将法向量的范围从[0,255]映射到[-1,1]
            //RGB对应xyz，TGAColor存储的是BGR
            res[2-i]=static_cast<float>(c[i])/255.f*2.f-1.f;
        }
        return res;
    }
    [[nodiscard]]const TGAImage& diffuse()const{
        return diffuse_map;
    }
    [[nodiscard]]const TGAImage& specular()const{
        return specular_map;
    }
};