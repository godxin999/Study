/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/10/1 9:36:07
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>

//R.20 使用unique_ptr或shared_ptr表示所有权
//weak_ptr是对std::shared所管理资源的非占有的引用，可以从某个std::weak_ptr创建出一个std::shared_ptr
//R.21 除非要共享所有权，否则能用unique_ptr就别用shared_ptr
//R.22/R.23 使用make_unique和make_shared来构造智能指针，原因有两个：
//1. 异常安全
//2. 使用make_shared/make_unique可以减少内存分配次数(使用new会进行两次分配，一次对象一次控制块，使用make_shared/make_unique只会进行一次分配)
//R.24 使用std::weak_ptr来打破循环引用
#include <memory>
struct Son;
struct Daughter;
struct Mother{
	~Mother(){
		std::cout << "Mother gone" << std::endl;
	}
	void setSon(const std::shared_ptr<Son> s){
		son = s;
	}
	void setDaughter(const std::shared_ptr<Daughter> d){
		daughter = d;
	}
	std::shared_ptr<Son> son;
	std::weak_ptr<Daughter> daughter;
};
struct Son{
	explicit Son(std::shared_ptr<Mother> m):mother(m){}
	~Son(){
		std::cout << "Son gone" << std::endl;
	}
	std::shared_ptr<Mother> mother;
};
struct Daughter{
	explicit Daughter(std::shared_ptr<Mother> m):mother(m){}
	~Daughter(){
		std::cout << "Daughter gone" << std::endl;
	}
	std::shared_ptr<Mother> mother;
};
void test1(){
	auto mother = std::make_shared<Mother>();
	auto son = std::make_shared<Son>(mother);
	auto daughter = std::make_shared<Daughter>(mother);
	mother->setSon(son);
	mother->setDaughter(daughter);
	//因为母子之间存在环状的std::shared_ptr，所以他们的引用计数永远不会变为0，所以他们的析构函数永远不会被调用
}

//函数签名                        语义
//func(std::unique_ptr<T>)        func拿走所有权
//func(std::unique_ptr<T>&)       func要重装T
//func(std::shared_ptr<T>)        func共享所有权
//func(std::shared_ptr<T>&)       func可能要重装T 
//func(const std::shared_ptr<T>&) func可能保有一份引用计数

//R.30 只在显式表达生存期语义时以智能指针为参数
using std::cout;

void asSmartPointerGood(std::shared_ptr<int>& sp){
	cout << "sp.use_count() = " << sp.use_count() << std::endl;
	sp.reset(new int(2023));//重装智能指针
	cout << "sp.use_count() = " << sp.use_count() << std::endl;
}
void asSmartPointerBad(std::shared_ptr<int>& sp){//参数语义表明可能要重装智能指针，但是函数没有这么做的意图
	*sp+=19;//如果只对共享指针的底层资源感兴趣，则使用原始指针
}
void test2(){
	auto sp1=std::make_shared<int>(2021);
	auto sp2=sp1;
	cout << "sp1.use_count() = " << sp1.use_count() << std::endl;//2
	asSmartPointerGood(sp1);
	cout<<"*sp1: "<<*sp1<<std::endl;
	cout << "sp1.use_count() = " << sp1.use_count() << std::endl;
	cout<<"*sp2: "<<*sp2<<std::endl;
	cout << "sp2.use_count() = " << sp2.use_count() << std::endl;
	asSmartPointerBad(sp2);
	cout<<"*sp2: "<<*sp2<<std::endl;
}
struct Widget{
	explicit Widget(int i):i(i){}
	int i;
};
void sink(std::unique_ptr<Widget> upw){//使用unique_ptr干些什么，然后丢弃
	cout << "sink: upw->i = " << upw->i << std::endl;
}
void reseat(std::unique_ptr<Widget>& upw){//重装unique_ptr
	upw.reset(new Widget(2023));
	cout << "reseat: upw->i = " << upw->i << std::endl;
}
void test3(){
	auto upw=std::make_unique<Widget>(2021);
	sink(std::move(upw));
	auto upw2=std::make_unique<Widget>(2022);
	reseat(upw2);
}
//对于std::shared_ptr
//void share(std::shared_ptr<T>);//共享所有权，只要在函数生存期内，就会保有一份引用计数(函数在生存期内T不会被析构)
//void reset(std::shared_ptr<T>&);//重装std::shared_ptr，不是共享所有者，因为没有改变引用计数，不能保证T在函数执行过程中存活，但是可以重装资源
//void mayshare(const std::shared_ptr<T>&);//也许会共享该资源，如果需要共享，函数可以通过拷贝shared_ptr将其保留

//R.37 不要传递从智能指针别名中获得的指针或引用(只有当你拥有共享资源的所有权时，你才可以访问该资源)
void Func(Widget& wid){

}
void shared(std::shared_ptr<Widget>& spw){
	//Func(*spw);//bad
	//两种解决方案，将函数签名改成void Func(std::shared_ptr<Widget> spw)，这样就拥有了共享所有权
	//另一种为auto keepAlive=spw;Func(*keepAlive);这样保证引用计数在调用函数前增加
}

auto gs=std::make_shared<Widget>(2021);
void test4(){
	shared(gs);
}

int main(){
	//test1();
	//test2();
	test3();


	return 0;
}