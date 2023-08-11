/*************************************************************************
	> File Name: template.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/11 18:59:49
 ************************************************************************/

#include <iostream>
using namespace std;

class Library{
public:
	void Run(){
		step1();
		if (step2()){
			step3();//支持变化，通过虚函数多态特性实现
		}
		for (int i = 0; i < 4; i++){
			step4();//支持变化，通过虚函数多态特性实现
		}
		step5();
	}
	virtual ~Library(){}
protected:
	void step1(){}//稳定
	void step3(){}//稳定
	void step5(){}//稳定
	virtual bool step2() = 0;//变化，子类可以重写
	virtual void step4() = 0;//变化，子类可以重写
};
