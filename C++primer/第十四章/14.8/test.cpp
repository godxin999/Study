/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/1 22:18:02
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <functional>
#include <map>
using namespace std;
//重载调用运算符
struct absInt{
	int operator()(int val)const{//函数调用运算符必须是成员函数
		return val<0?-val:val;
	}
};
void test1(){
	int i=-42;
	absInt absObj;//定义absInt类型对象
	int ui=absObj(i);//将i传递给absObj.operator()
	cout<<ui<<endl;//42
}
//函数对象类通常含有一些数据成员，其用于定制调用运算符的操作
class PrintString{
public:
	PrintString(ostream &o=cout,char c=' '):os(o),sep(c){}
	void operator()(const string &s){
		os<<s<<sep;
	}
private:
	ostream &os;//用于写入的目的流
	char sep;//用于将不同输出隔开的字符
};
void test2(){
	string s("Hello");
	PrintString printer;
	PrintString errors(cerr,'\n');//定义自己的分隔符
	errors(s);//在cerr中打印s，后面跟一个换行
	printer(s);//在cout中打印s，后面跟一个空格
	cout<<endl;
	vector<string> vs={"1","2","3","4","5"};
	for_each(vs.begin(),vs.end(),errors);//向for_each中传入自己定义的可调用对象
}
//lambda表达式是函数对象
class ShorterString{
public:
	bool operator()(const string &s1,const string &s2)const{
		return s1.size()<s2.size();
	}
};
void test3(){
	vector<string> words={"hello","world","C++"},words_copy(words);
	stable_sort(words.begin(),words.end(),[](const string &s1,const string &s2){//这个lambda表达式的行为类似于ShorterString类
		return s1.size()<s2.size();
	});
	for(auto i:words){
		cout<<i<<" ";
	}
	cout<<endl;
	stable_sort(words_copy.begin(),words_copy.end(),ShorterString());
	for(auto i:words){
		cout<<i<<" ";
	}
	cout<<endl;
}
//表示lambda及相应捕获行为的类
class SizeComp{
public:
	SizeComp(size_t n):sz(n){}//不具有默认构造函数，必须提供一个参数以进行构造
	bool operator()(const string& s)const{
		return s.size()<sz;
	}
private:
	size_t sz;//对应lambda通过值捕获的变量
};
void test4(){
	vector<string> words={"hello","world","C++"},words_copy(words);
	size_t sz=4;
	auto wc=find_if(words.begin(),words.end(),[sz](const string &s){
		return s.size()<sz;
	});
	cout<<*wc<<endl;
	auto wd=find_if(words.begin(),words.end(),SizeComp(sz));
	cout<<*wc<<endl;
}
//使用标准库容器定义的函数对象
void test5(){
	plus<int> intAdd;//可以执行int加法的函数对象
	negate<int> intNegate;//可以执行int取反的函数对象
	int sum=intAdd(10,20);
	cout<<sum<<endl;//30
	sum=intNegate(intAdd(10,20));
	cout<<sum<<endl;//-30
	sum=intAdd(10,intNegate(10));
	cout<<sum<<endl;//0
}
//在算法中使用标准库函数对象
void test6(){
	vector<string> vs={"a","ab","abc","acb","ac","ba","bca"};
	sort(vs.begin(),vs.end());//默认的排序使用operator<将序列升序排列
	for(auto i:vs){
		cout<<i<<" ";//a ab abc ac acb ba bca 
	}
	cout<<endl;
	sort(vs.begin(),vs.end(),greater<string>());//传入greater<string>函数对象，使排序执行>来进行排序
	for(auto i:vs){
		cout<<i<<" ";//bca ba acb ac abc ab a
	}
	cout<<endl;
}
void test7(){
	vector<string*> nameTable;
	string s1("a");
	string s2("ab");
	string s3("abc");
	string s4("acb");
	string s5("ac");
	string s6("ba");
	string s7("bca");
	nameTable.push_back(&s1);
	nameTable.push_back(&s2);
	nameTable.push_back(&s3);
	nameTable.push_back(&s4);
	nameTable.push_back(&s5);
	nameTable.push_back(&s6);
	nameTable.push_back(&s7);
	sort(nameTable.begin(),nameTable.end(),less<string*>());//按照指针地址升序排列
	for(auto i:nameTable){
		cout<<*i<<" ";//bca ba ac acb abc ab a 
	}
	cout<<endl;
}
//使用function类型为具有相同调用形式的函数建立索引
int add(int i,int j){
	return i+j;
}
struct divide{
	int operator()(int denominator,int divisor){
		return denominator/divisor;
	}
};
void test8(){
	auto mod=[](int i,int j){
		return i%j;
	};
	function<int(int,int)> f1=add;
	function<int(int,int)> f2=divide();
	function<int(int,int)> f3=[](int i,int j){
		return i*j;
	};
	cout<<f1(4,2)<<endl;//6
	cout<<f2(4,2)<<endl;//2
	cout<<f3(4,2)<<endl;//8
	map<string,function<int(int,int)>> binops={//建立索引
		{"+",add},
		{"-",std::minus<int>()},
		{"/",divide()},
		{"*",[](int i,int j){return i*j;}},
		{"%",mod}
	};
	//通过map返回的function对象的引用来调用对应的函数
	cout<<binops["+"](10,5)<<endl;//15
	cout<<binops["-"](10,5)<<endl;//5
	cout<<binops["*"](10,5)<<endl;//50
	cout<<binops["/"](10,5)<<endl;//2
	cout<<binops["%"](10,5)<<endl;//0
}
//使用函数指针或者lambda表达式为重载的函数建立索引
double add(double a,double b){
	return a+b;
}
void test9(){
	int (*fp)(int,int)=add;//fp指向add函数
	map<string,function<int(int,int)>> binops;
	//binops.insert({"+",fp});//使用函数指针建立索引
	binops.insert({"+",[](int i,int j){//使用lambda表达式建立索引
		return i+j;
	}});
	cout<<binops["+"](10,5)<<endl;//15
}
int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	//test6();
	//test7();
	//test8();
	test9();
}

