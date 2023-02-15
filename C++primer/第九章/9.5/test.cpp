/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/14 19:28:11
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <string>
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
//string的查找
void test5(){
	string name("AnnaBelle");
	auto pos=name.find("Anna");
	cout<<pos<<endl;//0
	pos=name.find("anna");
	cout<<(pos==string::npos)<<endl;//npos代表没有查找到
	string num("0123456789"),name2("r2d2");
	pos=name2.find_first_of(num);//在name2中查找第一个任何一个num中的字符的位置
	cout<<pos<<endl;//1，name2中第一个匹配的是下标为一的'2'
	string dept("03714p3");
	pos=dept.find_first_not_of(num);//在dept中查找第一个任何一个不是num中的字符的位置
	cout<<pos<<endl;//5，p为第一个不在num中的字符
}
//指定位置开始搜索
void test6(){
	string::size_type pos=0;
	string num("0123456789"),name("r2d2");
	while((pos=name.find_first_of(num,pos))!=string::npos){//只要能找到就继续循环
		cout<<"found number at index:"<<pos<<" element is "<<name[pos]<<endl;
		/*found number at index:1 element is 2
		found number at index:3 element is 2*/
		++pos;//不断递增
	}
}
//逆向搜索
void test7(){
	string river("Mississippi");
	auto pos1=river.find("is");
	auto pos2=river.rfind("is");//逆向第一次匹配"is"在下标为4的位置上
	cout<<pos1<<" "<<pos2<<endl;//1 4
}
//数值转换
void test8(){
	int i=42;
	string s=to_string(i);//将整型转换为string类型
	cout<<is_same<decltype(stod(s)),double>::value<<endl;//1,stod函数将string类型转换为double类型
	string s2="pi = 3.14";
	double d=stod(s2.substr(s2.find_first_of("+-.0123456789")));
	cout<<d<<endl;//3.14
	string a="3e5";
	cout<<stod(a)<<endl;//300000
	a="3e0";
	cout<<stod(a)<<endl;//3
	a="3e-5";
	cout<<stod(a)<<endl;//3e-05
	a=".056";
	cout<<stod(a)<<endl;//0.056
	a="0x667";
	cout<<stod(a)<<endl;//1639
	try{
		a="rprr";
		cout<<stod(a)<<endl;
	}
	catch(invalid_argument err){
		cout<<err.what()<<endl;//invalid stod argument
	}
	try{
		a="3e9999999999999999999999999999999999999999999999999999999999999999999999999999";
		cout<<stod(a)<<endl;
	}
	catch(out_of_range err){
		cout<<err.what()<<endl;//stod argument out of range
	}
}
int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	//test6();
	//test7();
	test8();

}

