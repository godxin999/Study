/*************************************************************************
	> File Name: item16.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/22 23:08:37
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <mutex>
#include <atomic>
#include <vector>
using namespace std;
class Polynomial{
public:
	using RootsType=vector<double>;
	/*
	RootsType roots()const{//虽然声明为const成员函数，但不是线程安全的
		if(!rootsAreVaild){
			rootsAreVaild=true;
		}
		return rootVals;
	}
	*/

	RootsType roots()const{
		lock_guard<mutex> g(m);//使用互斥锁保证线程安全
		if(!rootsAreVaild){
			rootsAreVaild=true;
		}
		return rootVals;
	}

private:
	mutable mutex m;
	mutable bool rootsAreVaild{false};
	mutable RootsType rootVals{};
};
//有时使用互斥锁开销较大，可以使用atomic
class foo{
public:
	void count()const noexcept{
		++callcount;
	}
private:
	mutable atomic<unsigned> callcount{0};
};
//如果需要同步的是单个的变量或者内存位置，可以使用atomic，如果需要两个以上的变量或内存位置作为一个单元来操作，应该使用互斥锁mutex
class widget{
public:
	int magicValue()const{
		lock_guard<mutex> g(m);
		if(cacheValid)return cacheValue;
		else{
			//auto val1=func1();
			//auto val2=func2();
			//cacheValue=val1+val2;
			cacheValid=true;
			return cacheValue;
		}
	}
private:
	mutable mutex m;
	mutable int cacheValue;
	mutable bool cacheValid{false};
};

int main(){






}