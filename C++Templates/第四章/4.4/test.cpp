/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/16 5:24:03
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <complex>
#include <cstring>
#include <type_traits>
#include <vector>
#include <array>
#include <unordered_set>

template <typename... T>
void print(const T&... args){
	(std::cout<<...<<args)<<std::endl;
}

template <typename T>
class addSpace{
private:
	const T& ref;
public:
	addSpace(const T& r):ref(r){}
	//或许使用sstream更好?
	friend std::ostream& operator<<(std::ostream& os,const addSpace<T>& s){
		return os<<s.ref<<' ';
	}
};

template <typename... Args>
void printWithSpace(Args... args){
	(std::cout<<...<<addSpace<Args>(args))<<std::endl;
}

template <typename... T>
void printDoubled(const T&... args){
	printWithSpace(args+args...);//将参数包中的所有表达式翻倍
}

void test1(){
	printDoubled(7.5,std::string("hello"),std::complex<float>(4,2));//15 hellohello (8,4) 
}

//对每个参数加一
template <typename... T>
void addOne(const T&... args){
	printWithSpace(args + 1 ...);//...不能紧跟在数值后面
	//printWithSpace((args + 1)...);//正确
}

void test2(){
	addOne(1,2,3,4,5);
}

//判断参数包中的参数是否为同类型
template <typename T,typename... Args>
constexpr bool isHomogeneous(T t,Args... args){//因为是值传递，所以可能发生类型退化
	return (std::is_same_v<T,Args>&&...);
}

void test3(){
	std::cout<<isHomogeneous(1,std::string("hello"),0.5)<<std::endl;//0
	std::cout<<isHomogeneous(1,2,3,4,5)<<std::endl;//1
}

//变参下标
template <typename C,typename... Idx>
void printElems(const C& coll,Idx... idx){
	printWithSpace(coll[idx]...);
}

void test4(){
	std::vector<int> v{1,5,6,3,8};
	printElems(v,0,3,4);//1 3 8 
}

//使用非类型模板参数的版本
template <std::size_t... Idx,typename C>
void printIdx(const C& coll){
	printWithSpace(coll[Idx]...);
}

void test5(){
	std::vector<int> v{1,5,6,3,8};
	printIdx<0,3,4>(v);//1 3 8 
}

//变参类模板
template <std::size_t...>
struct Indices{};

template<typename T,std::size_t... Idx>
void printByIdx(T t,Indices<Idx...>){
	printWithSpace(std::get<Idx>(t)...);
}

void test6(){
	std::array<std::string,5> arr={"hello","I","am","godxin999","!"};
	printByIdx(arr,Indices<1,2,3>());
}

//变参基类及其使用
class Customer{
private:
	std::string name="";
public:
	Customer(const std::string& n):name(n){}
	std::string getName()const{return name;}
};

struct CustomerEqual{
	bool operator()(const Customer& c1,const Customer& c2)const{
		return c1.getName()==c2.getName();
	}
};

struct CustomerHash{
	std::size_t operator()(const Customer& c)const{
		return std::hash<std::string>()(c.getName());//首先创建对象，然后调用operator()
	}
};

template <typename... Bases>
struct overLoader: Bases...{
	using Bases::operator()...;//展开Bases中的operator()，并将其放入当前类中
};

void test7(){
	using CustomerOP=overLoader<CustomerHash,CustomerEqual>;
	std::unordered_set<Customer,CustomerHash,CustomerEqual> coll;
	std::unordered_set<Customer,CustomerOP,CustomerOP> coll2;//传入CustomerOP后，编译器自动选择正确的operator()
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