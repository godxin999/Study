/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/25 16:37:30
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//位域
typedef unsigned Bit;
class File{
	//可能会压缩在同一个unsigned中
	Bit mode:2;//占两位
	Bit modified:1;//占一位
	Bit prot_owner:3;//占三位
	Bit prot_group:3;
	Bit prot_world:3;
public:
	enum modes{//三种模式，分别为8进制的1，2，3
		READ=01,
		WRITE=02,
		EXECUTE=03
	};
	File &open(modes m){
		mode|=READ;//设置可读
		if(m&WRITE){//如果可写
			//按照读写方式打开文件
		}
		return *this;
	}
	void close(){
		if(modified){
			//保存内容
		}
	}
	void write(){
		modified=1;//修改状态为真
	}
	bool isRead()const{
		return mode & READ;//确定是否可读
	}
	void setWrite(){
		mode|=WRITE;//设置可写
	}
};
//位域使用
enum Level:unsigned{
	FRESHMAN,SOPHOMORE,JUNIOR,SENIOR
};
enum Grade:unsigned{
	A,B,C,D
};
class Student{
public:
	Student(unsigned num,Level l,Grade g):num(num),level(l),grade(g){}
	void show(){
		cout<<"number: "<<num<<endl;
		cout<<"level: ";
		switch (level){
			case FRESHMAN:{
				cout<<"freshman"<<endl;
				break;
			}
			case SOPHOMORE:{
				cout<<"sophomore"<<endl;
				break;
			}
			case JUNIOR:{
				cout<<"junior"<<endl;
				break;
			}
			case SENIOR:{
				cout<<"senior"<<endl;
				break;
			}
		}
		cout<<"grade: ";
		switch (grade){
			case A:{
				cout<<"A"<<endl;
				break;
			}
			case B:{
				cout<<"B"<<endl;
				break;
			}
			case C:{
				cout<<"C"<<endl;
				break;
			}
			case D:{
				cout<<"D"<<endl;
				break;
			}
		}
	}
private:
	unsigned num:27;
	Level level:2;
	Grade grade:2;
};
void test1(){
	Student s(12345678,SOPHOMORE,B);
	cout<<"size of student: "<<sizeof(Student)<<endl;
	s.show();
}
//volatile关键字，指示编译器不要进行优化
void test2(){
	volatile int v;
	volatile double *dp;//dp指向一个volatile对象
	volatile char buf[100];//buf的每个元素都是volatile
	int *volatile vip;//vip是一个volatile指针，其指向int
	volatile int *ivp;//ivp是一个指针，其指向一个volatile int
	volatile int *volatile vivp;
	ivp=&v;//正确，ivp是一个指向volatile的指针
	vivp=&v;//正确，vivp是一个指向volatile的volatile指针
}
//不能使用合成的拷贝/移动构造函数及赋值运算符来初始化volatile对象或从volatile对象赋值
class foo{
public:
	//将形参类型指定为const volatile &就可以利用任意类型的foo进行拷贝或者赋值操作了
	foo(const volatile foo&);
	foo& operator=(volatile const foo&);
	foo& operator=(volatile const foo&)volatile;
};
//extern链接指示
extern "C" size_t strlen(const char *);//单语句链接指示
extern "C" {//复合语句链接指示
	int strcmp(const char*,const char*);
	char *strcat(char*,const char*);
}
extern "C" {
	#include "string.h"//头文件中的所有函数都被认为是由C编写的
}
//指向extern "C"函数的指针
void (*pf1)(int);
extern "C" void (*pf2)(int);
void test3(){
	//pf1=pf2;//链接错误，指向C函数的指针和指向C++函数的指针不是同种类型
}
//链接指示对整个声明都有效
extern "C" void f1(void(*)(int));//这里形参列表中的函数指针也只能指向C函数
//使用类型别名来给C++函数传入C函数指针
extern "C" typedef void FC(int);//FC是一个指向C函数的指针
void f2(FC *);//f2是C++函数，其参数为一个C函数指针
//导出C++代码到其他语言
extern "C" void print(){
	//print函数可以被C语言调用
}
//在C和C++中编译同一个文件可以使用条件编译
#ifdef __cplusplus
extern "C"
#endif
int strcmp(const char*,const char*);
//重载函数与链接指示
extern "C" void f(const char*);
//extern "C" void f(int);//错误，C语言不支持函数重载
//如果一组重载函数有一个是C函数，那么其余一定为C++函数
class X{
	void c(int){}
};
class Y{
	void c(double){}
};
extern "C" void c(char);
extern X c(int);
extern Y c(double);

int main(){
	//test1();
	//test2();
	test3();
}