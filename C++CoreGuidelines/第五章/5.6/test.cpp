/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/26 22:26:18
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>

//C.180 使用union来节省内存
//C.181 避免裸union
//C.182 使用匿名union来实现带标签联合体
//这里使用std::variant(带标签联合体)
#include <variant>

void test1(){
	std::variant<int,float> v;
	std::variant<int,float> w;

	int i=std::get<int>(v);//0
	std::cout<<i<<std::endl;

	v=12;//v包含int
	int j=std::get<int>(v);
	std::cout<<j<<std::endl;//12

	w=std::get<int>(v);
	w=std::get<0>(v);//根据索引访问，效果和上一行相同
	w=v;//效果和上一行相同

	try{
		auto val=std::get<float>(w);//w包含int而不是float，抛出异常
	}
	catch(std::bad_variant_access& err){
		std::cout<<err.what()<<std::endl;
	}

	v=5.5f;//切换为float
	v=5;//切换回int

	std::variant<std::string> v2("abc");
	//如果构造函数调用或者赋值调用没有二义性，转换就可以发生，即可以使用C风格字符串构造std::variant<std::string>或赋值给它
	v2="def";
}



int main(){
	test1();




	return 0;
}