/*************************************************************************
	> File Name: item4.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/31 22:47:04
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <type_traits>
#include <boost/type_index.hpp>
using namespace std;
//利用编译器诊断查看推导结果
template<typename T>
class TD;//声明一个模板但不定义，只要用要判断的变量的类型去实例化模板就能从报错中得到对应的推到类型
void test1(){
	const int i=42;
	auto x=i;
	auto y=&i;
	//TD<decltype(x)> xType;//error: implicit instantiation of undefined template 'TD<int>'
	//TD<decltype(y)> yType;//error: implicit instantiation of undefined template 'TD<const int *>'
}
//运行时输出类型
void test2(){
	const int i=42;
	auto x=i;
	auto y=&i;
	cout<<typeid(x).name()<<endl;//clang&msvc:int  g++:i(int)
	cout<<typeid(y).name()<<endl;//clang&msvc:int const * __ptr64  g++:PKi(pointer to const int)(PK:pointer to const)
}
//typeid的结果不一定是可靠的
template <typename T>
void f(const T &param){
	cout<<"T = "<<typeid(T).name()<<endl;
	cout<<"param = "<<typeid(param).name()<<endl;
}
struct widget{
	int a=1,b=1,c=1;
};
vector<widget> createvec(){
	vector<widget> v;
	v.push_back({1,2,3});
	return v;
}
void test3(){
	const auto vw=createvec();
	f(&vw[0]);
//clang&msvc: T = struct widget const * __ptr64   param = struct widget const * __ptr64
//g++: T = PK6widget   param = PK6widget(pointer to const widget)(6为名称字符串的长度)
}
//boost的类型推导
template <typename T>
void func(const T& param) {
	using boost::typeindex::type_id_with_cvr;
	cout << "T= " << type_id_with_cvr<T>().pretty_name() << endl;
	cout << "param= " << type_id_with_cvr<decltype(param)>().pretty_name() << endl;
}
void test4() {
	const auto vw = createvec();
	func(&vw[0]);
//msvc: T= struct widget const * __ptr64   param= struct widget const * __ptr64 const & __ptr64
}
int main(){
	//test1();
	//test2();
	//test3();
	test4();


}

