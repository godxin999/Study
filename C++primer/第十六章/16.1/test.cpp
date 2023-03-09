/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/6 18:28:35
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;
//实例化函数模板
template <typename T>
int compare1(const T &v1,const T &v2){//泛型代码的第一个原则，函数参数为const的引用
	if(v1<v2)return -1;//泛型代码的第二个原则，条件判断仅使用<比较运算
	if(v2<v1)return 1;
	return 0;
}
void test1(){
	cout<<compare1(0,1)<<endl;//实例化为int compare(const int&,const int&);
	vector<int> v1{1,2,3},v2{4,5,6};
	cout<<compare1(v1,v2)<<endl;//实例化为int compare(const vector<int>&,const vector<int>&);
}
//模板类型参数
template <typename T> T foo(T *p){
	T temp=*p;
	return temp;
}
void test2(){
	int a=100;
	int *p=&a;
	cout<<foo(p)<<endl;
}
//在在模板参数列表中，class和typename没有区别
template <class T,typename U> T calc(const T&,const U&);
//非类型模板参数
template <unsigned N,unsigned M>//实例化的时候会被用户提供的值或者编译器推断出的值所替代，这些值必须为常量表达式
int compare2(const char (&p1)[N],const char (&p2)[M]){//比较两个字符数组，两个模板参数分别为两个数组的长度
	return strcmp(p1,p2);
}
void test3(){
	cout<<compare2("hi","mom")<<endl;//编译器通过字面常量的大小来代替N和M，其中编译器会插入一个空字符作为结束，所以该函数会实例化为int compare(const char (&p1)[3],const char (&p2)[4]);
}
//函数模板可以被声明为inline或constexpr，说明符放在模板参数列表之后，返回类型之前
template <typename T> inline T min(const T&,const T&);
//类型无关，可移植的compare函数，使用less实现的小于
template <typename T> int compare(const T &v1,const T &v2){
	if(less<T>()(v1,v2))return -1;
	if(less<T>()(v2,v1))return 1;
	return 0;
}
void test4(){
	int a=100,b=200;
	int *p1=&a,*p2=&b;
	cout<<compare(p1,p2)<<endl;
}
//模板友元声明
template <typename T> class Pal;//前置声明
class C{
	friend class Pal<C>;//用C实例化的Pal是C的友元
	template <typename T> friend class Pal2;//Pal2的所有实例都是C的友元，这种情况无需前置声明
};
template <typename T> class C2{
	friend class Pal<T>;//C2的每个实例把相同类型实例化的Pal声明为友元
	template <typename X> friend class Pal2;//Pal2的所有实例都是C2每个实例的友元，为了让所有实例成为友元，友元声明中必须使用与类模板本身不同的模板参数
	friend class Pal3;//Pal3是一个非模板类，它是C2所有实例的友元
};
//模板参数声明为友元
template <typename Type>
class ObjectCounter{
	friend Type;
private:
	static ObjectCounter* getInstance(){
		if(m_instance==nullptr){
			m_instance=new ObjectCounter;
		}
		return m_instance;
	}
	void incCounter(){
		m_counter++;
		cout<<typeid(Type).name()<<" counter:"<<m_counter<<endl;
	}
	void decCounter(){
		m_counter--;
		cout<<typeid(Type).name()<<" counter:"<<m_counter<<endl;
	}
	int m_counter=0;
	static ObjectCounter* m_instance;
};
template <typename Type>
ObjectCounter<Type>* ObjectCounter<Type>::m_instance =nullptr;
class A{
public:
	A(){
		ObjectCounter<A>::getInstance()->incCounter();
	}
	~A(){
		ObjectCounter<A>::getInstance()->decCounter();
	}
};
class B{
public:
	B(){
		ObjectCounter<B>::getInstance()->incCounter();
	}
	~B(){
		ObjectCounter<B>::getInstance()->decCounter();
	}
};
void test5(){
	A a1;
	A a2;
	B b;
}
/*
class A counter:1
class A counter:2
class B counter:1
class B counter:0
class A counter:1
class A counter:0
*/
//模板类型别名
template <typename T> using twin=pair<T,T>;//可以固定一个或多个模板参数
template <typename T> using partNo=pair<T,unsigned>;
void test6(){
	twin<string> authors{"a","an"};
	cout<<authors.first<<" "<<authors.second<<endl;
	partNo<string> pn{"a",100};
	cout<<pn.first<<" "<<pn.second<<endl;
}
//类模板的static成员
template <typename T>
class Foo{
public:
	static size_t count(){return ctr;}
	void add(){++ctr;}
private:
	static size_t ctr;
};
template <typename T>
size_t Foo<T>::ctr=0;//定义并初始化ctr，所有相同类型的实例共享相同的静态成员
void test7(){
	Foo<int> fi;
	auto ct=Foo<int>::count();//实例化Foo<int>::count
	ct=fi.count();//使用Foo<int>::count
	Foo<string> fs;
	cout<<fi.count()<<endl;//0
	fi.add();
	Foo<int> fi2;
	cout<<fi2.count()<<endl;//1
	cout<<fs.count()<<endl;//0
}
//模板参数和作用域
typedef double D;
template <typename D,typename E>
void f(D d,E e){
	D temp=d;//typedef的类型参数D被模板参数隐藏
	cout<<typeid(D).name()<<endl;//int
	//double E;//错误，重声明模板参数E
}
void test8(){
	int a=100,b=1000;
	f(a,b);
}
/*
template <typename A,typename A>//错误，模板参数名不能重用
void f(){}
*/

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

