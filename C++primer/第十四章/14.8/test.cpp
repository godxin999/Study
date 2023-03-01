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



int main(){
	//test1();
	//test2();
	//test3();
	test4();


}

