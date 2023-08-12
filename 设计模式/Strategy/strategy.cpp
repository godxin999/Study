/*************************************************************************
	> File Name: strategy.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/12 23:45:27
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
class Context{};
class TaxStrategy{
public:
	virtual double calc(const Context& c)=0;
	virtual ~TaxStrategy(){}
};
class CNTex:public TaxStrategy{
public:
	virtual double calc(const Context& c){
		cout<<"CNTex"<<endl;
		return 0.0;
	}
};
class USTex:public TaxStrategy{
public:
	virtual double calc(const Context& c){
		cout<<"USTex"<<endl;
		return 0.0;
	}
};
//扩展
class DETex:public TaxStrategy{
public:
	virtual double calc(const Context& c){
		cout<<"DETex"<<endl;
		return 0.0;
	}
};

class SalesOrder{
private:
	TaxStrategy* strategy;
public:
	//工厂模式
	SalesOrder(StrategyFactory* strategyFactory){
		this->strategy=strategyFactory->NewStrategy();
	}
	~SalesOrder(){
		delete this->strategy;
	}
	double CalculateTax(){
		Context context;
		return strategy->calc(context);
		double val=strategy->calc(context);//多态调用
	}
};

int main(){






}

