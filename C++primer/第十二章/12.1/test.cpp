/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/24 9:49:54
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <memory>
using namespace std;
//make_shared函数
void test1(){
	shared_ptr<int> p1=make_shared<int> (42);
	shared_ptr<string> p2=make_shared<string> (10,'9');
	shared_ptr<int> p3=make_shared<int> ();//不传递参数则值初始化为0
	cout<<*p1<<endl;
	cout<<*p2<<endl;
	cout<<*p3<<endl;//0
	auto p4=make_shared<vector<int> > ();//利用auto接收make_shared返回的shared_ptr类型的返回值
}
//智能指针的拷贝和赋值
void test2(){
	auto p=make_shared<int> (42);
	auto q(p);//p、q指向相同对象
	auto r=make_shared<int> (100);
	r=q;//对r赋值，q的引用计数增加，r原来所指向的对象会被自动释放
}
//new
void test3(){
	string *ps=new string(10,'9');
	int *pi=new int(1024);
	vector<int> *pv=new vector<int>{0,1,2,3,4,5,6,7,8,9};
	cout<<*pi<<endl;
	cout<<*ps<<endl;
	cout<<pv->size()<<endl;//10
	int *pi2=new int;//未定义
	int *pi3=new int();//值初始化为0
	string *ps2=new string;//默认初始化为""
	string *ps3=new string();//值初始化为""
	auto p1=new auto(100);
	auto p2=new auto("hello");
	//auto p3=new auto{1,2,3};//错误，括号中只能有单个初始化器
}
//new分配const对象
void test4(){
	const int *pci=new const int(1024);
	const string *pcs=new const string;
}
//内存耗尽
void test5(){
	int *p1=new int;//如果分配失败将会抛出bad_alloc异常
	int *p2=new(nothrow) int;//如果分配失败将会返回空指针，nothrow即不抛出异常
}
//delete
void test6(){
	int i,*pi1=&i,*pi2=nullptr;
	double *pd=new double(13),*pd2=pd;
	//delete i;//错误，i不是一个指针
	//delete pi1;//未定义，pi1指向一个局部变量
	delete pd;
	//delete pd2;//错误，pd2指向的内存已经被释放了
	delete pi2;
	const int *pci=new const int(1024);
	delete pci;//可以释放一个const对象
}
//多个指针指向同一块内存
void test7(){
	int *p(new int(42));
	auto q=p;
	delete p;//q、p指向同一块内存，删除p时q也变得无效了
	p=nullptr;
	//cout<<*q<<endl;//未定义值
}
//shared_ptr和new结合使用
void test8(){
	//shared_ptr<int> p1=new int(1024);//接受指针参数的智能指针构造函数时explicit的，所以不能隐式把new返回的指针转换为智能指针
	shared_ptr<int> p2(new int(1024));//使用直接初始化方式
}
/*
shared_ptr<int> clone(int p){
	return new int(p);//不能进行隐式转换
}
*/
shared_ptr<int> clone(int p){
	return shared_ptr<int>(new int(p));//必须将shared_ptr显式绑定到要返回的指针上
}
//不要混合使用普通指针和智能指针
void f(shared_ptr<int> ptr){

}
void test9(){
	int *x(new int(1024));
	f(shared_ptr<int>(x));//合法，但是函数结束后内存会被释放
	int j=*x;//错误，未定义的值
	cout<<j<<endl;
	
	shared_ptr<int> p(new int(1024));
	f(p);
	int i=*p;//正确，此时引用计数为1
	cout<<i<<endl;
}
//不要使用get来初始化另一个智能指针或者为智能指针赋值
void test10(){
	shared_ptr<int> p(new int(42));
	int *q=p.get();//获取裸指针，使用时要注意不要让它管理的指针被释放
	{
		auto temp=shared_ptr<int> (q);
	}//程序块结束，q被销毁，其指向的内存被销毁
	int i=*p;//此时p指向的内存已经被释放，且在p销毁时，这块内存会被二次delete
	cout<<i<<endl;//错误，未定义的值
}
//其他shared_ptr操作
void test11(){
	shared_ptr<int> p(new int(1));
	p.reset(new int(1024));//将智能指针指向新的对象，如果原来指向的对象引用计数为1，则释放所指向的对象
	if(!p.unique()){//如果当前引用计数不为1，不能直接改变对象的值
		p.reset(new int (*p));//分配新的拷贝，此时p的引用计数为1，我们可以改变对象的值
		*p+=1024;
	}
	cout<<*p<<endl;
}
//使用自定义的删除操作
struct item{
	int num=1;
};
void d(item* a){
	cout<<"use custom deletor"<<endl;
	delete a;
}
void test12(){
	item* a=new item;
	shared_ptr<item> p(a,d);//为了使用自定义的删除器，在创建时还要传入删除器d，其接受一个item*的参数
}
//unique_ptr
void test13(){
	unique_ptr<int> p1(new int(42));
	//unique_ptr<int> p2(p);//unique_ptr不支持拷贝
	unique_ptr<int> p3;
	//p3=p;//unique_ptr不支持赋值
	unique_ptr<int> p4(p1.release());//p4初始化为p原来保存的指针，p被置空
	cout<<*p4<<endl;
	p3.reset(p4.release());//p3初始化为p4原来保存的指针，p4被置空
	cout<<*p3<<endl;
	auto p=p3.release();//如果不用另一个智能指针来接受release返回值，那么就要手动释放该指针
	delete p;
}
//如果一个unique_ptr将要被销毁，那么这个unique_ptr可以被拷贝和赋值
unique_ptr<int> uclone(int p){
	return unique_ptr<int> (new int(p));
}
unique_ptr<int> uclone2(int p){
	unique_ptr<int> ret(new int(p));
	return ret;
}
//向unique_ptr传递删除器
void test14(){
	item* a=new item;
	unique_ptr<item,decltype(d)*> p(a,d);//unique_ptr传递自定义删除器时还要在尖括号中提供删除器类型
}
//weak_ptr
void test15(){
	auto p=make_shared<int>(42);
	weak_ptr<int> wp(p);//将weak_ptr绑定到shared_ptr上
	if(shared_ptr<int> np=wp.lock()){//使用lock函数检查所绑定对象是否为空，如果非空则返回指向wp的对象的shared_ptr，否则返回空shared_ptr
		cout<<*np<<endl;
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
	//test8();
	//test9();
	//test10();
	//test11();
	//test12();
	//test13();
	//test14();
	test15();


}

