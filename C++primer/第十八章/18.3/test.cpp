/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/21 22:15:32
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <memory>
using namespace std;
//继承的构造函数与多重继承
struct base1{
	base1()=default;
	base1(const string&);
	base1(shared_ptr<int>);
};
struct base2{
	base2()=default;
	base2(const string&);
	base2(int);
};
/*
//编译不会报错，但是d1试图从两个基类中继承相同的构造函数
struct d1:public base1,public base2{
	using base1::base1;
	using base2::base2;
};
*/
struct d2:public base1,public base2{
	using base1::base1;
	using base2::base2;
	d2(const string &s):base1(s),base2(s){}//自定义一个接受string的构造函数
	d2()=default;
};


int main(){






}

