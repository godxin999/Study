#pragma once

#include <cmath>
#include <ostream>


template <typename T>
struct Vec2{
    union{
        struct{
            T x,y;
        };
        struct{
            T u,v;
        };
        T raw[2];
    };
    Vec2():u(0),v(0){}
    Vec2(T _u,T _v):u(_u),v(_v){}
    Vec2 operator+(const Vec2& V)const{
        return Vec2(u+V.u,v+V.v);
    }
    Vec2 operator-(const Vec2& V)const{
        return Vec2(u-V.u,v-V.v);
    }
    Vec2 operator*(float f)const{
        return Vec2(u*f,v*f);
    }
    template<typename>
    friend std::ostream& operator<<(std::ostream& s,const Vec2* v);
};

template <typename T>
std::ostream& operator<<(std::ostream& s,const Vec2<T>& v){
    s<<"("<<v.x<<","<<v.y<<")\n";
    return s;
}

template <typename T>
struct Vec3{
    union{
        struct{
            T x,y,z;
        };
        struct{
            T ivert,iuv,inorm;
        };
        T raw[3];
    };
    Vec3():x(0),y(0),z(0){}
    Vec3(T _x,T _y,T _z):x(_x),y(_y),z(_z){}
    Vec3 operator^(const Vec3& v)const{
        return Vec3(y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x);
    }
    Vec3 operator+(const Vec3& v)const{
        return Vec3(x+v.x,y+v.y,z+v.z);
    }
    Vec3 operator-(const Vec3& v)const{
        return Vec3(x-v.x,y-v.y,z-v.z);
    }
    Vec3 operator*(float f)const{
        return Vec3(x*f,y*f,z*f);
    }
    T operator*(const Vec3& v)const{
        return x*v.x+y*v.y+z*v.z;
    }
    [[nodiscard]]float norm()const{
        return std::sqrt(x*x+y*y+z*z);
    }
    Vec3& normalize(T l=1){
        *this=(*this)*(l/norm());
        return *this;
    }
    template<typename>
    friend std::ostream& operator<<(std::ostream& s,const Vec3* v);
};

template <typename T>
std::ostream& operator<<(std::ostream& s,const Vec3<T>& v){
    s<<"("<<v.x<<","<<v.y<<","<<v.z<<")\n";
    return s;
}

using Vec2f=Vec2<float>;
using Vec2i=Vec2<int>;
using Vec3f=Vec3<float>;
using Vec3i=Vec3<int>;