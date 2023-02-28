/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/27 10:11:50
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//行为像值的类
/*
class HasPtr{
public:
	HasPtr(const string &s=string()):ps(new string(s)),i(0){}
	HasPtr(const HasPtr &p):ps(new string(*p.ps)),i(p.i){}//拷贝string，而不是指针
	HasPtr& operator=(const HasPtr&);
	~HasPtr(){
		delete ps;
	}
private:
	string *ps;
	int i;
};
HasPtr& HasPtr::operator=(const HasPtr &rhs){//需要保证自赋值的结果正确
	auto newp=new stirng(*rhs.ps);//首先拷贝，如果先delete会发生从已经释放的内存中拷贝数据的错误
	delete ps;
	ps=newp;
	i=rhs.i;
	return *this;
}
*/
//行为像指针的类
class HasPtr{
public:
	HasPtr(const string &s=string()):ps(new string(s)),i(0),use(new size_t(1)){}//创建新计数器并置一
	HasPtr(const HasPtr &p):ps(p.ps),i(p.i),use(p.use){++*use;}//这里不拷贝p.ps指向的string，而是直接拷贝指针，并且递增计数器
	HasPtr& operator=(const HasPtr&);
	~HasPtr();//可能有复数对象指向同一块内存，所以不能直接delete指针
private:
	string *ps;
	int i;
	size_t *use;//记录多少个对象共享ps成员
};
HasPtr::~HasPtr(){
	if(--*use==0){//如果引用计数递减为0，就delete指针
		delete ps;
		delete use;
	}
}
HasPtr& HasPtr::operator=(const HasPtr &rhs){
	++*rhs.use;//首先递增右侧对象的引用计数
	if(--*use==0){//对左侧运算对象进行析构检验
		delete ps;
		delete use;
	}
	ps=rhs.ps;//拷贝右侧对象到本对象
	i=rhs.i;
	use=rhs.use;
	return *this;//返回本对象
}



int main(){






}

