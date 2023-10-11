/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/10/11 17:22:33
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <memory>
//E.3 只对错误处理使用异常
void getIndex(std::vector<const std::string>& vec,const std::string& str){
	try{
		for(auto i=0;i<vec.size();++i){
			if(vec[i]==str){
				throw i;//不好，常规控制流和异常控制流没有分开
			}
		}
	}catch(int i){
		return i;
	}
	return -1;
}
//E.14 应该使用专门涉及的用户定义类型而非内置类型来作为异常
class InputSubsystemException:public std::runtime_error{
	//输入子系统的用户可以通过catch(InputSubsystemException& e)来专门捕获这一异常
	//可以继续派生来完善异常层次结构
	const char* what()const noexcept override{
		return "InputSubsystemException";
	}
};
//E.15 通过引用从层次结构中捕获异常(防止按值捕获所导致的切片)
//应当使用const&来捕获异常，除非需要修改异常时使用引用来捕获
//如果需要在异常处理器中重新抛出异常e，只需要使用throw即可，不需要使用throw e(e会被复制)

//E.13 在直接拥有对象时决不抛出异常
void leak(int x){
	auto *p=new int{7};
	auto *pa=new int[100];
	if(x<0)throw std::runtime_error("x<0");//抛出异常后，p和pa将会泄漏
	delete p;
	delete []pa;
}
void leak2(int x){
	auto p1=int{7};
	auto p=std::make_unique<int>(7);
	auto pa=std::vector<int>(100);
	if(x<0)throw std::runtime_error("x<0");
	//消除了delete调用
}
//E.30 不要使用异常规格(带参数的动态异常规格在C++17被删除，不带参数的动态异常规格在C++20被删除)
/*int use(int x)throw(X,Y){
	//...
}*/
//E.31 正确排列catch子句的顺序
//应该从具体到一般来组织异常处理器
class DivisionByZeroException:public std::exception{
	const char* what()const noexcept override{
		return "DivisionByZeroException";
	}
};
void func(){
	try{
		//抛出异常
	}
	catch(const DivisionByZeroException& e){
		//...
	}
	catch(const std::exception& e){
		//...
	}
	catch(...){//捕获所有其他的异常
		//...
	}
}

int main(){


	return 0;
}