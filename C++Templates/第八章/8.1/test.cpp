/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/21 15:21:12
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>

template <unsigned p,unsigned d>
struct DoIsPrime{
	static constexpr bool value=(p%d!=0)&&DoIsPrime<p,d-1>::value;
};
template <unsigned p>//偏特化用于终止递归
struct DoIsPrime<p,2>{
	static constexpr bool value=(p%2!=0);
};
template <unsigned p>
struct IsPrime{
	static constexpr bool value=DoIsPrime<p,p/2>::value;
};
template <>
struct IsPrime<2>{static constexpr bool value=true;};
template <>
struct IsPrime<3>{static constexpr bool value=true;};
template <>
struct IsPrime<1>{static constexpr bool value=false;};
template <>
struct IsPrime<0>{static constexpr bool value=false;};



int main(){
	//递归深度最多1024
	std::cout<<IsPrime<1013>::value<<std::endl;//1
	return 0;
}