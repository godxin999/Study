/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/20 23:26:22
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <map>

//C.20 如果能避免定义默认操作，就别定义它们
class Named_map{
public:
private:
	//因为std::string和std::map已经定义了默认构造和拷贝构造，所以编译器可以自己生成默认构造和拷贝构造
	std::map<int, int> mii;
	std::string name;
};
void test1(){
	Named_map nm;//默认构造
	Named_map nm2{nm};//拷贝构造
}
//C.21 如果定义或删除了任何默认操作(拷贝、移动和析构)，就对所有默认操作进行定义或删除
#include <cstddef>
class BigArray{
public:
	BigArray(std::size_t len):len_(len),data_(new int[len]){}
	~BigArray(){
		delete[] data_;
	}
private:
	std::size_t len_;
	int* data_;
};
void test2(){
	BigArray ba1(1000);
	BigArray ba2(1000);
	ba2=ba1;//默认的拷贝赋值操作对ba1进行了浅拷贝，仅复制了指针，没有复制指针指向的内容，导致两个对象指向同一块内存，析构时会出错
}



int main(){
	test1();
	test2();



	return 0;
}