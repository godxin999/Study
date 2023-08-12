/*************************************************************************
	> File Name: test2.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/12 18:57:16
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//多模板参数的部分特例化
template<typename T1,typename T2>
class Myclass{
	
};
template <typename T>
class Myclass<T,T>{

};
template <typename T>
class Myclass<T,int>{

};
template<typename T1,typename T2>
class Myclass<T1*,T2*>{

};
//为消除歧义提供单独的特例化版本来处理相同类型的指针
template <typename T>
class Myclass<T*,T*>{

};

int main(){
	Myclass<int,float> mif;
	Myclass<float,float> mff;
	Myclass<float,int> mfi;
	Myclass<int*,float*> mp;
	//Myclass<int,int> mii;//错误，同时匹配Myclass<T,T>和<Myclass <T,int>
	//Myclass<int*,int*> mpipi;//错误，同时匹配Myclass<T,T>和<Myclass <T*,T*>
	Myclass<int*,int*> mpipi;//通过部分特例化模板解除歧义
	return 0;
}
