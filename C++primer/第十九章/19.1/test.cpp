/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/22 15:07:58
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstdlib>
using namespace std;
//重载new和delete运算符
void *operator new(size_t size){
	if(void *mem=malloc(size)){
		return mem;
	}
	else{
		throw bad_alloc();
	}
}
void operator delete(void *mem)noexcept{
	free(mem);
}
//定位new表达式
class A{
public:
	A(){
		cout<<"A's constructor"<<endl;
	}
	~A(){
		cout<<"A's destructor"<<endl;
	}
	void show(){
		cout<<"num: "<<num<<endl;
	}
private:
	int num;
};
void test1(){
	char mem[100];//提前分配一块内存
	mem[0]='A';
	mem[1]='\0';
	mem[2]='\0';
	mem[3]='\0';
	cout<<(void*)mem<<endl;
	A* p=new(mem) A;//使用定位new，在mem处构造一个A对象
	cout<<p<<endl;
	p->show();//65
	p->~A();//因为对象的空间不会自动释放，所以定位new必须显式进行析构
}

int main(){
	test1();
}