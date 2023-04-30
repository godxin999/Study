/*************************************************************************
	> File Name: item18.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/29 20:14:45
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <memory>
#include <type_traits>
using namespace std;

class Investment{
public:
	virtual ~Investment()=default;
};
class Stock:public Investment{};
class Bond:public Investment{};
class RealEstate:public Investment{};
//自定义unique_ptr的删除器
auto delInvmt=[](Investment* pInvestment){//使用lambda表达式自定义unique_ptr的删除器
	//makeLogEntry(pInvestment);
	delete pInvestment;
};
void delInvmt2(Investment* pInvestment){//使用函数自定义unique_ptr删除对象
	//makeLogEntry(pInvestment);
	delete pInvestment;
}
template <typename... Ts>
unique_ptr<Investment,decltype(delInvmt)>//C++14可以直接使用auto进行推导
makeInvestment(Ts&&... param){
	unique_ptr<Investment,decltype(delInvmt)> pInv(nullptr,delInvmt);
	//unique_ptr<Investment,decltype(delInvmt2)*> pInv2(nullptr,delInvmt2);
	//unique_ptr<Investment,void(*)(Investment*)> pInv3(nullptr,delInvmt2);
	//根据需要创建对象并将unique_ptr绑定到该指针上
	if(/*a Stock object should be create*/){
		pInv.reset(new Stock(std::forward<Ts>(param)...));
	}
	else if(/*a Bond object should be create*/){
		pInv.reset(new Bond(std::forward<Ts>(param)...));
	}
	else if(/*a RealEstate object should be create*/){
		pInv.reset(new RealEstate(std::forward<Ts>(param)...));
	}
	return pInv;
}
class EmptyClass{};
class AnInt:public EmptyClass{
	int base;
};
void test1(){
	cout<<sizeof(EmptyClass)<<endl;//1，空类会占1字节
	cout<<sizeof(AnInt)<<endl;//4，父类为空，但是子类包含一个int，所以为4字节
	//根据该结果，如果使用函数自定义unique_ptr删除器，那么使用函数指针构成的unique_ptr返回值将会占用16字节，使用lambda表达式自定义unique_ptr删除器将会占用8字节，因为lambda表达式被翻译为一个不含对象的类，其和第一个参数共占8字节
	unique_ptr<Investment,decltype(delInvmt)> pInv(nullptr,delInvmt);
	unique_ptr<Investment,decltype(delInvmt2)*> pInv2(nullptr,delInvmt2);
	unique_ptr<Investment,void(*)(Investment*)> pInv3(nullptr,delInvmt2);
	cout<<sizeof(pInv)<<endl;//8
	cout<<sizeof(pInv2)<<endl;//16
	cout<<sizeof(pInv3)<<endl;//16
}
//unique_ptr指向数组时可以用下标运算符[]访问数组数据
void test2(){
	unique_ptr<int[]> pArr(new int[10]);
	unique_ptr<int> pi(new int(100));
	for(int i=0;i<10;++i){
		pArr[i]=i;
	}
	cout<<pArr[5]<<endl;//5
	cout<<*pi<<endl;//100
}
int main(){
	//cout<<is_same<decltype(delInvmt2)*,void(*)(Investment*)>::value<<endl;
	//test1();
	test2();
}