#pragma once

#include "geometry.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>


class Model{
private:
    std::vector<Vec3f> verts_;
    std::vector<std::vector<int>> faces_;
public:
    explicit Model(const char* filename){
        std::ifstream in(filename,std::ios::in);
        if(in.fail())return;
        std::string line;
        while(!in.eof()){
            std::getline(in,line);
            std::istringstream iss(line);
            char temp;
            //读取顶点数据
            if(!line.compare(0,2,"v ")){
                iss>>temp;
                Vec3f v;
                for(float & i : v.raw){
                    iss>>i;
                }
                verts_.push_back(v);
            }
            //读取面数据
            else if(!line.compare(0,2,"f ")){
                std::vector<int> f;
                int itemp,idx;
                iss>>temp;
                while(iss>>idx>>temp>>itemp>>temp>>itemp){
                    //obj文件的索引从1开始，所以需要减一
                    --idx;
                    f.push_back(idx);
                }
                faces_.push_back(f);
            }
        }
    }
    ~Model()=default;
    int nverts(){
        return static_cast<int>(verts_.size());
    }
    int nfaces(){
        return static_cast<int>(faces_.size());
    }
    Vec3f& vert(int i){
        return verts_[i];
    }
    std::vector<int>& face(int idx){
        return faces_[idx];
    }
};