/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/27 11:34:57
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;

class HasPtr{
	friend void swap(HasPtr&,HasPtr&);
public:
	HasPtr(const string &s=string()):ps(new string(s)),i(0){}
	HasPtr(const HasPtr &p):ps(new string(*p.ps)),i(p.i){}//拷贝string，而不是指针
	HasPtr(HasPtr &&p)noexcept:ps(p.ps),i(p.i){p.ps=nullptr;}
	HasPtr& operator=(HasPtr);//因为参数是一个非引用参数，传入参数时，左值将会被拷贝，右值将会被移动，所以单一的赋值运算符实现了拷贝赋值运算符和移动赋值运算符
	~HasPtr(){
		delete ps;
	}
private:
	string *ps;
	int i;
};
inline void swap(HasPtr &lhs,HasPtr &rhs){
	swap(lhs.ps,rhs.ps);//交换指针而不是string数据
	swap(lhs.i,rhs.i);
}
//使用拷贝和交换的赋值运算符自动就是异常安全的，且能正确处理自赋值
HasPtr& HasPtr::operator=(HasPtr rhs){//rhs按值传递，使用拷贝构造函数将右侧运算对象拷贝rhs中
	swap(*this,rhs);//交换左侧运算对象和局部运算对象rhs的内容
	return *this;//rhs被销毁从而delete了rhs中的指针
}


int main(){






}

