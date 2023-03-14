/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/14 15:58:43
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <tuple>
#include <vector>
#include <list>
using namespace std;
//tuple
void test1(){
	tuple<size_t,size_t,size_t> threeD;
	tuple<string,vector<double>,int,list<int>> someVal("contents",{3,14,2.718},42,{0,1,2,3,4,5});
	tuple<int,int,int> threei{1,2,3};//tuple的构造函数是explicit的，所以只能使用直接初始化的语法
	auto item=make_tuple("10202929",3,20.00);//item为一个tuple，类型推断为tuple<const char*,int,double>
}
//访问tuple的成员
void test2(){
	auto item=make_tuple("10202929",3,20.00);
	auto book=get<0>(item);//返回item的第一个成员
	auto cnt=get<1>(item);//返回item的第二个成员
	auto price=get<2>(item)/cnt;//根据第三个成员求出单价
	get<2>(item)*=0.8;//书的总价打折20%
}
//根据辅助模板类获取信息
void test3(){
	auto item=make_tuple("10202929",3,20.00);
	typedef decltype(item) trans;//获取tuple的类型
	size_t sz=tuple_size<trans>::value;//通过tuple_size的静态成员value来获取tuple中成员的数量
	cout<<sz<<endl;//3
	tuple_element<1,trans>::type cnt=get<1>(item);//通过tuple_element的type成员和索引值来获取对应成员的类型
}
//比较tuple时要保证每对成员使用==或<是合法的
void test4(){
	tuple<string,string> ss{"12d","2ds"};
	tuple<int,int> ii{1,2};
	//cout<<(ss<ii)<<endl;//错误，不能比较int和string
	tuple<size_t,size_t> uu{13,32};
	cout<<(uu<ii)<<endl;//0
	tuple<unsigned,unsigned> uu2{1,2};
	cout<<(uu2==ii)<<endl;//1
}
int main(){
	//test1();
	//test2();
	//test3();
	test4();


}

