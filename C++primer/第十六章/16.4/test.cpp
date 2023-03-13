/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/12 22:29:40
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <sstream>
using namespace std;
//�ɱ����ģ��
template<typename T,typename... Args>//Args��ģ�������
void foo(const T&,const Args& ... rest){}//rest�Ǻ���������
void test1(){
	int i=0;
	double d=3.14;
	string s="how now brown cow";
	foo(i,s,42,d);//ʵ����Ϊfoo(const int&,const string&,const int&,const double&);
	foo(s,42,"hi");//ʵ����Ϊfoo(const string&,const int&,const char[3]&);
	foo(d,s);//ʵ����Ϊfoo(const double&,const stirng&);
	foo("hi");//ʵ����Ϊfoo(const char[3]&);
}
//sizeof...�������ȡ�������Ĳ�������
template <typename ...Args>
void g(Args ... args){
	cout<<sizeof...(Args)<<" ";//������Ͳ�������Ŀ
	cout<<sizeof...(args)<<endl;//���������������Ŀ
}
void test2(){
	g(1,2,3);//3 3
	g(1,'a');//2 2
}
//�ɱ��������ͨ���ǵݹ�ģ���Ҫ����һ���ǿɱ������������ֹ�ݹ�
template <typename T>
ostream& print(ostream& os,const T &t){//���������ǿɱ����ģ�壬��ô�ɱ�����汾��������޵ݹ�
	return os<<t;
}
template <typename T,typename... Args>
ostream& print(ostream& os,const T &t,const Args&... rest){
	os<<t<<" ";
	return print(os,rest...);
}
void test3(){
	string s="100";
	int i=6;
	print(cout,i,s,42);//6 100 42
}
//����չ
template <typename T>
string debug_rep(const T &t){
	ostringstream ret;
	ret<<t;
	return ret.str();
}
template <typename T>
string debug_rep(T *p){
	ostringstream ret;
	ret<<"pointer: "<<p;
	if(p){
		ret<<" "<<debug_rep(*p);
	}
	else ret<<" null pointer";
	return ret.str();
}
template <typename... Args>
ostream& errorMsg(ostream &os,const Args&... rest){
	return print(os,debug_rep(rest)...);//��rest�е�ÿ��Ԫ�ص���debug_rep
	//return print(os,debug_rep(rest...));//������չrest�ȼ��ڵ��ý��ܿɱ������debug_rep
}
void test4(){
	errorMsg(cerr,1,"a",3.14);//1 pointer: a a 3.14
}
//ģ��������
template <typename T>
int compare(const T &a,const T &b){//���ԱȽ�������������
	return a<b;
}
template <size_t N,size_t M>
int compare(const char (&p1)[N],const char (&p2)[M]){//���Դ����ַ������泣��
	return strcmp(p1,p2);
}
template<>//�ռ�����ָ�����ǽ�ҪΪԭģ�������ģ������ṩʵ�Σ���Ҫԭģ�����������������
int compare(const char* const &p1,const char* const &p2){//������ģ�壬�����ַ������ָ��
	return strcmp(p1,p2);
}
void test5(){
	const char *p1="hi",*p2="mom";
	cout<<compare(p1,p2)<<endl;
	cout<<compare("hi","mom")<<endl;

}
//��������Ա��������
template <typename T>
struct Foo{
	Foo(const T &t=T()):mem(t){}
	void Bar(){
		cout<<"����ʵ����ģ��"<<endl;
	}
	T mem;
};
template <>
void Foo<int>::Bar(){
	cout<<"������Foo<int>::Bar"<<endl;
}
void test6(){
	Foo<string> fs;
	fs.Bar();//����ʵ����ģ��
	Foo<int> fi;
	fi.Bar();//������Foo<int>::Bar
}

int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	test6();
}

