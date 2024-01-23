#pragma once
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstring>
#include <iostream>

template <size_t Dim,typename T>
struct vec{
    vec()=default;
    vec(const vec& v)=default;
    vec& operator=(const vec& v)=default;
    vec(vec&& v)=default;
    vec& operator=(vec&& v)=default;
    ~vec()=default;
    explicit vec(T fill){
        for(size_t i=0;i<Dim;++i){
            data[i]=fill;
        }
    }
    vec& operator=(T fill){
        for(size_t i=0;i<Dim;++i){
            data[i]=fill;
        }
        return *this;
    }
    vec(const std::initializer_list<T> &lst){
        assert(lst.size()==Dim&&"incorrect initializer list size");
        size_t i=0;
        for(const auto &it:lst){
            data[i++]=it;
        }
    }
    vec& operator=(const std::initializer_list<T> &lst){
        assert(lst.size()==Dim&&"incorrect initializer list size");
        size_t i=0;
        for(const auto &it:lst){
            data[i++]=it;
        }
        return *this;
    }
    T& operator[](const size_t i){
        assert(i<Dim&&"index out of range");
        return data[i];
    }
    const T& operator[](const size_t i)const{
        assert(i<Dim&&"index out of range");
        return data[i];
    }
    T norm2()const{
        return *this**this;
    }
    T norm()const{
        return std::sqrt(norm2());
    }
    vec<Dim,T>& normalize(){
        *this=*this/norm();
        return *this;
    }
    T data[Dim]{};    
};

template <typename T>
struct vec<2,T>{
    vec()=default;
    vec(T X,T Y):x(X),y(Y){}
    explicit vec(T fill){
        x=y=fill;
    }
    vec& operator=(T fill){
        x=y=fill;
        return *this;
    }
    vec(const vec& v)=default;
    vec& operator=(const vec& v)=default;
    vec(vec&& v)=default;
    vec& operator=(vec&& v)=default;
    ~vec()=default;
    T& operator[](const size_t i){
        assert(i<2&&"index out of range");
        return i==0?x:y;
    }
    const T& operator[](const size_t i)const{
        assert(i<2&&"index out of range");
        return i==0?x:y;
    }
    vec(const std::initializer_list<T> &lst){
        assert(lst.size()==2&&"incorrect initializer list size");
        x=*lst.begin();
        y=*(lst.begin()+1);
    }
    vec& operator=(const std::initializer_list<T> &lst){
        assert(lst.size()==2&&"incorrect initializer list size");
        x=*lst.begin();
        y=*(lst.begin()+1);
        return *this;
    }
    T norm2()const{
        return x*x+y*y;
    }
    T norm()const{
        return std::sqrt(norm2());
    }
    vec<2,T>& normalize(){
        *this=*this/norm();
        return *this;
    }
    T x{},y{};
};

template <typename T>
struct vec<3,T>{
    vec()=default;
    vec(T X,T Y,T Z):x(X),y(Y),z(Z){}
    explicit vec(T fill){
        x=y=z=fill;
    }
    vec& operator=(T fill){
        x=y=z=fill;
        return *this;
    }
    vec(const vec& v)=default;
    vec& operator=(const vec& v)=default;
    vec(vec&& v)=default;
    vec& operator=(vec&& v)=default;
    ~vec()=default;
    vec(const std::initializer_list<T> &lst){
        assert(lst.size()==3&&"incorrect initializer list size");
        x=*lst.begin();
        y=*(lst.begin()+1);
        z=*(lst.begin()+2);
    }
    vec& operator=(const std::initializer_list<T> &lst){
        assert(lst.size()==3&&"incorrect initializer list size");
        x=*lst.begin();
        y=*(lst.begin()+1);
        z=*(lst.begin()+2);
        return *this;
    }
    T& operator[](const size_t i){
        assert(i<3&&"index out of range");
        return i==0?x:(i==1?y:z);
    }
    const T& operator[](const size_t i)const{
        assert(i<3&&"index out of range");
        return i==0?x:(i==1?y:z);
    }
    T norm2()const{
        return x*x+y*y+z*z;
    }
    T norm()const{
        return std::sqrt(norm2());
    }
    vec<3,T>& normalize(){
        *this=*this/norm();
        return *this;
    }
    T x{},y{},z{};
};

//-----------------------------------------------------------------------------

template <size_t Dim,typename T>
vec<Dim,T> operator-(const vec<Dim,T> &v){
    vec<Dim,T> ret;
    for(size_t i=0;i<Dim;++i){
        ret[i]=-v[i];
    }
    return ret;
}

template <size_t Dim,typename T>
T operator*(const vec<Dim,T> &lhs,const vec<Dim,T> &rhs){
    T ret=T();
    for(size_t i=0;i<Dim;++i){
        ret+=lhs[i]*rhs[i];
    }
    return ret;
}

template <size_t Dim,typename T>
vec<Dim,T> operator+(vec<Dim,T> lhs,const vec<Dim,T> &rhs){
    for(size_t i=0;i<Dim;++i){
        lhs[i]+=rhs[i];
    }
    return lhs;
}

template <size_t Dim,typename T>
vec<Dim,T> operator-(vec<Dim,T> lhs,const vec<Dim,T> &rhs){
    for(size_t i=0;i<Dim;++i){
        lhs[i]-=rhs[i];
    }
    return lhs;
}

template <size_t Dim,typename T,typename U>
requires std::is_arithmetic_v<U>
vec<Dim,T> operator*(vec<Dim,T> lhs,U rhs){
    for(size_t i=0;i<Dim;++i){
        lhs[i]*=rhs;
    }
    return lhs;
}

template <size_t Dim,typename T,typename U>
requires std::is_arithmetic_v<U>
vec<Dim,T> operator/(vec<Dim,T> lhs,U rhs){
    for(size_t i=0;i<Dim;++i){
        lhs[i]/=rhs;
    }
    return lhs;
}

template <size_t Len,size_t Dim,typename T>
vec<Len,T> embed(const vec<Dim,T> &v,T fill=1){
    assert(Len>Dim&&"invalid embedding");
    vec<Len,T> ret;
    for(size_t i=0;i<Len;++i){
        ret[i]=i<Dim?v[i]:fill;
    }
    return ret;
}

template <size_t Len,size_t Dim,typename T>
vec<Len,T> proj(const vec<Dim,T>& v){
    assert(Len<Dim&&"invalid projection");
    vec<Len,T> ret;
    for(size_t i=0;i<Len;++i){
        ret[i]=v[i];
    }
    return ret;
}

template <typename T>
vec<3,T> cross(const vec<3,T> &v1,const vec<3,T> &v2){
    return {
        v1.y*v2.z-v1.z*v2.y,
        v1.z*v2.x-v1.x*v2.z,
        v1.x*v2.y-v1.y*v2.x
    };
}

template <size_t Dim,typename T>
vec<Dim,T> normalize(const vec<Dim,T> &v){
    return v/std::sqrt(v*v);
}

template <size_t Dim,typename T>
std::ostream& operator<<(std::ostream &out,const vec<Dim,T> &v){
    for(size_t i=0;i<Dim;++i){
        out<<v[i]<<" ";
    }
    return out;
}

//-------------------------------------------------------------------------------------

template <size_t DimRows,size_t DimCols,typename T>
class mat;

template <size_t Dim,typename T>
struct dt{
    static T det(const mat<Dim,Dim,T> &m){
        T ret=0;
        for(size_t i=0;i<Dim;++i){
            ret+=m[0][i]*m.cofactor(0,i);
        }
        return ret;
    }
};

template <typename T>
struct dt<1,T>{
    static T det(const mat<1,1,T> &m){
        return m[0][0];
    }
};

template <size_t DimRows,size_t DimCols,typename T>
class mat{
private:
    vec<DimCols,T> rows[DimRows]{};
public:
    mat()=default;
    mat(const mat& m)=default;
    mat& operator=(const mat& m)=default;
    mat(mat&& m)=default;
    mat& operator=(mat&& m)=default;
    explicit mat(T fill){
        for(size_t i=0;i<DimRows;++i){
            rows[i]=fill;
        }
    }
    mat(const std::initializer_list<std::initializer_list<T>> &lst){
        assert(lst.size()==DimRows&&"incorrect initializer list size");
        size_t i=0;
        for(const auto &it:lst){
            assert(it.size()==DimCols&&"incorrect initializer list size");
            rows[i++]=it;
        }
    }
    mat& operator=(const std::initializer_list<std::initializer_list<T>> &lst){
        assert(lst.size()==DimRows&&"incorrect initializer list size");
        size_t i=0;
        for(const auto &it:lst){
            assert(it.size()==DimCols&&"incorrect initializer list size");
            rows[i++]=it;
        }
        return *this;
    }
    ~mat()=default;
public:
    vec<DimCols,T>& operator[](const size_t i){
        assert(i<DimRows&&"index out of range");
        return rows[i];
    }
    const vec<DimCols,T>& operator[](const size_t i)const{
        assert(i<DimRows&&"index out of range");
        return rows[i];
    }
    vec<DimRows,T> col(const size_t i)const{
        assert(i<DimCols&&"index out of range");
        vec<DimRows,T> ret;
        for(size_t j=0;j<DimRows;++j){
            ret[j]=rows[j][i];
        }
        return ret;
    }
    void set_col(size_t i,const vec<DimRows,T> &v){
        assert(i<DimCols&&"index out of range");
        for(size_t j=0;j<DimRows;++j){
            rows[j][i]=v[j];
        }
    }
    mat<DimCols,DimRows,T> transpose(){
        mat<DimCols,DimRows,T> ret;
        for(size_t i=0;i<DimCols;++i){
            ret[i]=this->col(i);
        }
        return ret;
    }
    [[nodiscard]]int ncols()const{
        return DimCols;
    }
    [[nodiscard]]int nrows()const{
        return DimRows;
    }
};

template <size_t Dim,typename T>
class mat<Dim,Dim,T>{
private:
    vec<Dim,T> rows[Dim]{};
public:
    mat()=default;
    mat(const mat& m)=default;
    mat& operator=(const mat& m)=default;
    mat(mat&& m)=default;
    mat& operator=(mat&& m)=default;
    ~mat()=default;
    explicit mat(T fill){
        for(size_t i=0;i<Dim;++i){
            rows[i]=fill;
        }
    }
    mat(const std::initializer_list<std::initializer_list<T>> &lst){
        assert(lst.size()==Dim&&"incorrect initializer list size");
        size_t i=0;
        for(const auto &it:lst){
            assert(it.size()==Dim&&"incorrect initializer list size");
            rows[i++]=it;
        }
    }
    mat& operator=(const std::initializer_list<std::initializer_list<T>> &lst){
        assert(lst.size()==Dim&&"incorrect initializer list size");
        size_t i=0;
        for(const auto &it:lst){
            assert(it.size()==Dim&&"incorrect initializer list size");
            rows[i++]=it;
        }
        return *this;
    }
public:
    vec<Dim,T>& operator[](const size_t i){
        assert(i<Dim&&"index out of range");
        return rows[i];
    }
    const vec<Dim,T>& operator[](const size_t i)const{
        assert(i<Dim&&"index out of range");
        return rows[i];
    }
    vec<Dim,T> col(const size_t i)const{
        assert(i<Dim&&"index out of range");
        vec<Dim,T> ret;
        for(size_t j=0;j<Dim;++j){
            ret[j]=rows[j][i];
        }
        return ret;
    }
    void set_col(size_t i,const vec<Dim,T> &v){
        assert(i<Dim&&"index out of range");
        for(size_t j=0;j<Dim;++j){
            rows[j][i]=v[j];
        }
    }
    static mat<Dim,Dim,T> identity(){
        mat<Dim,Dim,T> ret;
        for(size_t i=0;i<Dim;++i){
            for(size_t j=0;j<Dim;++j){
                ret[i][j]=i==j?1:0;
            }
        }
        return ret;
    }
    T det()const{
        return dt<Dim,T>::det(*this);
    }
    T get_minor(size_t row,size_t col)const{
        mat<Dim-1,Dim-1,T> ret;
        for(size_t i=0;i<Dim-1;++i){
            for(size_t j=0;j<Dim-1;++j){
                ret[i][j]=rows[i<row?i:i+1][j<col?j:j+1];
            }
        }
        return ret.det();
    }
    T cofactor(size_t row,size_t col)const{
        assert(row<Dim&&"index out of range");
        assert(col<Dim&&"index out of range");
        return get_minor(row,col)*((row+col)&1?-1:1);
    }
    mat<Dim,Dim,T> adjugate(){
        mat<Dim,Dim,T> ret;
        for(size_t i=0;i<Dim;++i){
            for(size_t j=0;j<Dim;++j){
                ret[i][j]=cofactor(i,j);
            }
        }
        return ret;
    }
    mat<Dim,Dim,T> invert_transpose(){
        mat<Dim,Dim,T> ret=adjugate();
        //利用adj(A)*A=|A|*I求逆矩阵
        T tmp=ret[0]*rows[0];
        assert(tmp!=0&&"matrix is not invertible");
        return ret/tmp;
    }
    mat<Dim,Dim,T> invert(){
        return invert_transpose().transpose();
    }
    mat<Dim,Dim,T> transpose(){
        mat<Dim,Dim,T> ret;
        for(size_t i=0;i<Dim;++i){
            ret[i]=this->col(i);
        }
        return ret;
    }
    [[nodiscard]]int ncols()const{
        return Dim;
    }
    [[nodiscard]]int nrows()const{
        return Dim;
    } 
};

//----------------------------------------------------------------------

template <size_t DimRows,size_t DimCols,typename T>
mat<DimRows,DimCols,T> operator-(const mat<DimRows,DimCols,T> &m){
    mat<DimRows,DimCols,T> ret;
    for(size_t i=0;i<DimRows;++i){
        ret[i]=-m[i];
    }
    return ret;
}

template <size_t DimRows,size_t DimCols,typename T>
vec<DimRows,T> operator*(const mat<DimRows,DimCols,T> & lhs,const vec<DimCols,T> &rhs){
    vec<DimRows,T> ret;
    for(size_t i=0;i<DimRows;++i){
        ret[i]=lhs[i]*rhs;
    }
    return ret;
}

template <size_t R1,size_t C1,size_t C2,typename T>
mat<R1,C2,T> operator*(const mat<R1,C1,T> &lhs,const mat<C1,C2,T> &rhs){
    mat<R1,C2,T> ret;
    for(size_t i=0;i<R1;++i){
        for(size_t j=0;j<C2;++j){
            ret[i][j]=lhs[i]*rhs.col(j);
        }
    }
    return ret;
}

template <size_t DimRows,size_t DimCols,typename T>
mat<DimRows,DimCols,T> operator*(mat<DimRows,DimCols,T> lhs,T rhs){
    for(size_t i=0;i<DimRows;++i){
        lhs[i]=lhs[i]*rhs;
    }
    return lhs;
}

template <size_t DimRows,size_t DimCols,typename T>
requires std::is_arithmetic_v<T>
mat<DimRows,DimCols,T> operator/(mat<DimRows,DimCols,T> lhs,T rhs){
    for(size_t i=0;i<DimRows;++i){
        lhs[i]=lhs[i]/rhs;
    }
    return lhs;
}

template <size_t DimRows,size_t DimCols,typename T>
mat<DimRows,DimCols,T> operator+(mat<DimRows,DimCols,T> lhs,const mat<DimRows,DimCols,T> &rhs){
    for(size_t i=0;i<DimRows;++i){
        lhs[i]=lhs[i]+rhs[i];
    }
    return lhs;
}

template <size_t DimRows,size_t DimCols,typename T>
mat<DimRows,DimCols,T> operator-(mat<DimRows,DimCols,T> lhs,const mat<DimRows,DimCols,T> &rhs){
    for(size_t i=0;i<DimRows;++i){
        lhs[i]=lhs[i]-rhs[i];
    }
    return lhs;
}

template <size_t DimRows,size_t DimCols,typename T>
std::ostream& operator<<(std::ostream &out,const mat<DimRows,DimCols,T> &m){
    for(size_t i=0;i<DimRows;++i){
        out<<m[i]<<"\n";
    }
    return out;
}

//------------------------------------------------------------------------------------
using Vec2f=vec<2,float>;
using Vec3f=vec<3,float>;
using Vec4f=vec<4,float>;
using Vec2i=vec<2,int>;
using Vec3i=vec<3,int>;
using Mat44f=mat<4,4,float>;
using Mat33f=mat<3,3,float>;
using Mat44i=mat<4,4,int>;
using Mat33i=mat<3,3,int>;

//------------------------------------------------------------------------------------
inline constexpr float max_float=std::numeric_limits<float>::max();
inline constexpr float min_float=std::numeric_limits<float>::lowest();
inline constexpr float pi=3.14159265358979323846F;
