/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/24 8:41:58
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>


//std::for_each()的实现
template  <typename Iter,typename Callable>
void foreach(Iter current,Iter end,Callable op){
	while(current!=end){
		op(*current);
		++current;
	}
}
void func(int i){
	std::cout<<"func(int i)"<<i<<std::endl;
}
class FuncObj{
public:
	void operator()(int i)const{
		std::cout<<"FuncObj::operator()(int i)"<<i<<std::endl;
	}
};
void test1(){
	std::vector<int> p={2,3,5,7};
	foreach(p.begin(),p.end(),func);//按值传递时函数名退化为函数指针
	foreach(p.begin(),p.end(),&func);//传入函数指针
	foreach(p.begin(),p.end(),FuncObj());
	foreach(p.begin(),p.end(),[](int i){std::cout<<"[](int i)"<<i<<std::endl;});
}

//使用std::invoke来调用函数对象
#include <functional>
#include <utility>

template <typename Iter,typename Callable,typename... Args>
void foreach2(Iter current,Iter end,Callable op,const Args&... args){
	while(current!=end){
		std::invoke(op,args...,*current);
		++current;
	}
}
class MyClass{
public:
	void memfunc(int i)const{
		std::cout<<"MyClass::memfunc(int i)"<<i<<std::endl;
	}
};
void test2(){
	std::vector <int> p={2,3,5,7};
	MyClass obj;
	//对于类成员指针，std::invoke会把args...中的第一个参数当作this对象，args...中其余的参数则被当作常规参数传递给可调用对象
	foreach2(p.begin(),p.end(),&MyClass::memfunc,obj);
	foreach2(p.begin(),p.end(),[](const std::string& p,int i){
		std::cout<<"[](const std::string& p,int i)"<<p<<i<<std::endl;
	},"value is ");//第四个参数被作为lambda的第一个参数
}
//std::invoke的常规用法是封装一个单独的函数调用，此时可以通过完美转发来传递可调用对象和参数
template <typename Callable,typename... Args>
decltype(auto) call(Callable&& op,Args&&... args){//为了能返回引用，使用decltype(auto)
	//如果可调用对象的返回类型为void，会出现问题，因为void是不完整类型
	//decltype(auto) ret{std::invoke(std::forward<Callable>(op),std::forward<Args>(args)...)};
	//return ret;
	return std::invoke(std::forward<Callable>(op),std::forward<Args>(args)...);//如果想把返回值存到临时变量中，则也需要使用decltype(auto)
}
//此时有两种方式，一种为在返回前声明一个对象，并在其析构函数中实现期望的行为
//另一种为分别实现void和非void的情况
#include <type_traits>
template <typename Callable,typename... Args>
decltype(auto) call2(Callable&& op,Args&&... args){
	if constexpr(std::is_same_v<std::invoke_result_t<Callable,Args...>,void>){
		std::invoke(std::forward<Callable>(op),std::forward<Args>(args)...);
		return;
	}
	else{
		decltype(auto) ret{std::invoke(std::forward<Callable>(op),std::forward<Args>(args)...)};
		return ret;
	}
}
void test3(){
	call2([](int i){std::cout<<"[](int i)"<<i<<std::endl;},42);
	std::cout<<call2([](int i){std::cout<<"[](int i)"<<i<<std::endl;return i;},42)<<std::endl;
}


int main(){
	//test1();
	//test2();
	test3();


	return 0;
}