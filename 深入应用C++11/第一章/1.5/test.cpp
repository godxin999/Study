/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/7/5 19:59:34
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <functional>
#include <cstring>
#include <vector>
using namespace std;
//std::function
void func(){
	cout<<__FUNCTION__<<endl;
}
class Foo{
public:
	static int foo_func(int a){
		cout<<__FUNCTION__ <<"("<< a <<")"<< endl;
		return a;
	}
};
class Bar{
public:
	int operator()(int a){
		cout<<__FUNCTION__ <<"("<< a <<")"<< endl;
		return a;
	}
};
void test1(){
	function<void(void)> fr1=func;
	fr1();
	function<int(int)> fr2=Foo::foo_func;
	cout<<fr2(123)<<endl;
	Bar bar;
	fr2=bar;
	cout<<fr2(123)<<endl;
}
//std::function作为回调函数
class A{
	std::function<void(void)> callback_;
public:
	A(const function<void()>& f):callback_(f){}

	void notify(){
		callback_();//回调到上层
	}
};
class foo{
public:
	void operator()(){
		cout<<__FUNCTION__<<endl;
	}
};
void test2(){
	foo f;
	A aa(f);
	aa.notify();//operator()
}
//std::function作为函数入参
void call_when_even(int x,const std::function <void(int)> &f){
	if(!(x&1)){
		f(x);
	}
}
void output(int x){
	cout<<x<<" ";
}
void test3(){
	for (int i = 0; i <10;++i ) {
		call_when_even(i,output);//0 2 4 6 8
	}
	cout<<endl;
}
//std::bind
void output2(int x,int y){
	cout<<x<<" "<<y<<endl;
}
void test4(){ 
	std::bind(output2,1,2)();//1 2
	std::bind(output2,std::placeholders::_1,2)(1);//1 2
	std::bind(output2,2,std::placeholders::_1)(1);//2 1
	std::bind(output2,2,std::placeholders::_2)(1,2);//2 2，第一个参数被吞掉了
	std::bind(output2,std::placeholders::_1,std::placeholders::_2)(1,2);//1 2
	std::bind(output2,std::placeholders::_2,std::placeholders::_1)(1,2);// 2 1

}
class B{
public:
	int i_=0;
	void output(int x,int y){
		cout<<x<<" "<<y<<endl;
	}
};
void test5(){
	B b;
	std::function<void(int,int)> fr=std::bind(&B::output,&b,std::placeholders::_1,std::placeholders::_2);
	fr(1,2);//1 2
	std::function<int&(void)> fr_i=std::bind(&B::i_,&b);
	fr_i()=123;
	cout<<b.i_<<endl;//123
}
void test6(){
	vector<int> v(100);
	for(int i=0;i<100;++i){
		v[i]=i;
	}
	int count=count_if(v.begin(),v.end(),std::bind(less<int>(),10,std::placeholders::_1));//计数元素值大于10的个数
	int count2=count_if(v.begin(),v.end(),std::bind(less<int>(),std::placeholders::_1,10));//计数元素值小于10的个数
	cout<<count<<" "<<count2<<endl;//89 10
}
//组合使用std::bind
void test7(){
	vector<int> v(10);
	for(int i=0;i<10;++i){
		v[i]=i;
	}
	auto f=std::bind(logical_and<int>(),std::bind(greater<int>(),std::placeholders::_1,5),std::bind(less_equal<int>(),std::placeholders::_1,10));//判断一个数是否大于5且小于等于10
	int count=count_if(v.begin(),v.end(),f);
	cout<<count<<endl;//4
}
int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	//test6();
	test7();
}