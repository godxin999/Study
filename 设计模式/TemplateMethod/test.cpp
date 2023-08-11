/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/11 18:46:41
 ************************************************************************/

#include "template.cpp"
using namespace std;

class Application:public Library{
protected:
	//子类重写虚函数实现变化
	virtual bool step2(){
		return true;
	}
	virtual void step4(){}
};

int main(){
	//通过多态性实现变化
	Library *p = new Application();
	p->Run();
	delete p;
	return 0;
}