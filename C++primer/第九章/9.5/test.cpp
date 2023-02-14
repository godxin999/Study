/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/14 19:28:11
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//string构造方式
void test1(){
	const char *cp="Hello World!!!";
	char n[]={'H','i'};
	string s1(cp);//拷贝cp中的字符到s1中
	cout<<s1<<endl;//Hello World!!!
	string s2(n,2);//从n中拷贝两个字符
	cout<<s2<<endl;//Hi
	string s3(n);//未定义，n不是以空字符结束的
	cout<<s3<<endl;//Hi郤?
	string s4(cp+6,5);//从cp[6]开始拷贝5个字符
	cout<<s4<<endl;//World
	string s5(s1,6,5);//从s1[6]开始拷贝5个字符
	cout<<s5<<endl;//World
	string s6(s1,6);//从s1[6]开始拷贝到s1末尾
	cout<<s6<<endl;//World!!!
	string s7(s1,6,20);//只拷贝到s1的末尾
	cout<<s7<<endl;//World!!!
	try{
		string s8(s1,16);//抛出越界异常，因为16大于了s1的大小
	}
	catch(out_of_range err){
		cout<<err.what()<<endl;//invalid string position
	}
}
//substr操作
void test2(){
	string s("Hello World");
	string s2=s.substr(0,5);
	cout<<s2<<endl;//Hello
	string s3=s.substr(6);
	cout<<s3<<endl;//World
	string s4=s.substr(6,11);
	cout<<s4<<endl;//World
	try{
		string s5=s.substr(12);//抛出越界异常，因为12大于了s的大小
	}
	catch(out_of_range err){
		cout<<err.what()<<endl;//invalid string position
	}
}
//接受下标版本的insert和erase
void test3(){
	string s="Hello World";
	s.insert(s.size(),5,'!');
	cout<<s<<endl;//Hello World!!!!!
	s.erase(s.size()-5,5);
	cout<<s<<endl;//Hello World
	s="";
	const char *cp="Stately, plump Buck";
	s.assign(cp,7);//用cp中前7个字符替换s的内容
	cout<<s<<endl;//Stately
	s.insert(s.size(),cp+7);//在s串后插入cp剩余的字符
	cout<<s<<endl;//Stately, plump Buck
	string s1="aaa",s2="bbb";
	s1.insert(0,s2);//在位置0之前插入s2的拷贝
	cout<<s1<<endl;//bbbaaa
	s1.insert(0,s2,0,s2.size());//在位置0之前插入s2从0到s2.size()之间的字符
	cout<<s1<<endl;//bbbbbbaaa
}
//append和replace函数
void test4(){
	string s="C++ Primer",s2=s;
	s.insert(s.size()," 4th Ed.");
	s2.append(" 4th Ed.");//等价造作
	s.erase(11,3);
	s.insert(11,"5th");
	s2.replace(11,3,"5th");//使用替换操作等价于上面两条操作
}

int main(){
	//test1();
	//test2();
	//test3();
	test4();


}

