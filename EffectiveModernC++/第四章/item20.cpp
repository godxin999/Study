/*************************************************************************
	> File Name: item20.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/2 13:39:56
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <memory>
#include <unordered_map>
using namespace std;
//wek_ptr
class widget{
public:
	explicit widget(int a):a(a){}
	int a;
};
void printwidget(weak_ptr<widget> &wpw){
	auto spw=wpw.lock();//使用lock成员函数获取指向对象的shared_ptr，并且递增引用计数，weak_ptr无效时返回空shared_ptr
	cout<<spw.use_count()<<endl;
	if(spw){
		cout<<"widget->a="<<spw->a<<endl;
	}
	else{
		cout<<"spw is nullptr"<<endl;
	}
}
void test1(){
	auto spw=make_shared<widget>(10);
	cout<<spw.use_count()<<endl;//1
	weak_ptr<widget> wpw(spw);//使用shared_ptr构造wek_ptr，shared_ptr的引用计数不会增加
	printwidget(wpw);//2
	spw=nullptr;//shared_ptr指向的对象会被析构，但是控制块仍然存在，不会被自动释放
	if(wpw.expired()){//如果失效
		cout<<"expired"<<endl;//expired
	}
	printwidget(wpw);//0
	try{
		shared_ptr<widget> spw2(wpw);//使用失效的weak_ptr创建shared_ptr会抛出bad_weak_ptr异常
	}
	catch(const std::exception &ex){
		cout<<"exception:"<<ex.what()<<endl;//exception:bad_weak_ptr
	}
}
unique_ptr<const widget> loadwidget(int wid){
	return make_unique<const widget>(wid);
}
shared_ptr<const widget> fastloadwidget(int wid){
	static std::unordered_map<int,weak_ptr<const widget>> cache;//可能会积累过期的weak_ptr
	auto objptr=cache[wid].lock();
	if(!objptr){//如果为空shared_ptr
		objptr=loadwidget(wid);//调用loadwidget返回unique_ptr，并将其转换为shared_ptr
		cache[wid]=objptr;//加入缓存
	}
	return objptr;
}
//智能指针的循环引用
class B;
class A{
public:
	A()=default;
	~A()=default;
	shared_ptr<B> b_ptr;
private:
	int a;
};
class B{
public:
	B()=default;
	~B()=default;
	shared_ptr<A> a_ptr;
private:
	int b;
};
void test2(){
	{
		auto a=make_shared<A>();//a:rc=1
		auto b=make_shared<B>();//b:rc=1
		a->b_ptr=b;//b:rc=2
		b->a_ptr=a;//a:rc=2
		cout<<a.use_count()<<" "<<b.use_count()<<endl;//2 2
	}
	//a:rc=1
	//b:rc=1
	//析构函数没有调用，发生了内存泄漏
}
class D;
class C{
public:
	C()=default;
	~C()=default;
	shared_ptr<D> d_ptr;
private:
	int c;
};
class D{
public:
	D()=default;
	~D()=default;
	weak_ptr<C> c_ptr;//更改为weak_ptr就不会发生内存泄漏问题
private:
	int d;
};
void test3(){
	{
		auto c=make_shared<C>();//c:rc=1
		auto d=make_shared<D>();//d:rc=1
		c->d_ptr=d;//d:rc=2
		d->c_ptr=c;//c:rc=1
		cout<<c.use_count()<<" "<<d.use_count()<<endl;//1 2
	}
	//c:rc=0
	//d:rc=0
}

int main(){
	//test1();
	test2();
	test3();
}