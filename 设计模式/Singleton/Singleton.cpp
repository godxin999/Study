/*************************************************************************
	> File Name: Singleton.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/1 9:20:15
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;

class Singleton{
public:
	static Singleton& getInstance(int val){
		static Singleton instance(val);
		return instance;
	}
	void print(){
		std::cout<<val<<std::endl;
	}
private:
	int val;
	Singleton(int val):val(val){}
	Singleton(const Singleton&)=delete;
	Singleton& operator=(const Singleton&)=delete;
};




int main(){
	Singleton &s=Singleton::getInstance(10);
	s.print();//10
	Singleton &s1=Singleton::getInstance(20);
	s1.print();//10
	return 0;
}