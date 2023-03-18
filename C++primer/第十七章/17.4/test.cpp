/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/17 19:58:13
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <cmath>
#include <random>
using namespace std;
//使用随机数引擎
void test1(){
	default_random_engine e;
	for(int i=0;i<10;++i){
		cout<<e()<<" ";
	}
}
//使用分布类型的对象
void test2(){
	uniform_int_distribution<unsigned> u(0,9);//分布类型，此类型生成均匀分布的unsigned值，并规定范围为(0,9)
	default_random_engine e;
	for(int i=0;i<10;++i){
		cout<<u(e)<<" ";//分布类型使用引擎参数来生成随机数，并将其映射到指定的分布
	}
}
//获取引擎类型的范围
void test3(){
	default_random_engine e;
	cout<<"min: "<<e.min()<<" max: "<<e.max()<<endl;//min: 0 max: 4294967295
}
//要想使用一个随机数发生器一直生成随机数序列，那么需要把随机数生成器定义为静态的
vector<unsigned> good_randVec(){
	static default_random_engine e;
	static uniform_int_distribution<unsigned> u(0,9);
	vector<unsigned> ret;
	for(int i=0;i<10;++i){
		ret.push_back(u(e));
	}
	return ret;
}
void test4(){
	for(auto i:good_randVec()){
		cout<<i<<" ";
	}
}
//使用随机数种子
void test5(){
	default_random_engine e1;
	default_random_engine e2(2147483646);
	default_random_engine e3;
	e3.seed(32767);//使用seed成员设置种子
	default_random_engine e4(32767);
	for(int i=0;i!=100;++i){
		if(e1()==e2()){//具有不同种子的随机数发生器会生成不同的序列
			cout<<"unseeded match at iteration: "<<i<<endl;
		}
		if(e3()==e4()){//具有相同种子的随机数发生器会生成相同的序列
			cout<<"seeded differs at iteration: "<<i<<endl;
		}
	}
}
//生成随机实数
void test6(){
	default_random_engine e;
	//uniform_real_distribution<double> u(0,1);//范围为(0,1)的浮点值
	uniform_real_distribution<> u(0,1);//默认生成double值
	for(int i=0;i<10;++i){
		cout<<u(e)<<" ";
	}
}
//生成正态分布
void test7(){
	default_random_engine e;
	normal_distribution<> n(4,1.5);//生成均值为4，标准差为1.5的正态分布
	vector<unsigned> vals(9);
	for(int i=0;i<200;++i){
		unsigned v=lround(n(e));//舍入到最近的整数
		if(v<vals.size()){//统计每个数出现的自处
			++vals[v];
		}
	}
	for(int j=0;j!=vals.size();++j){
		cout<<j<<": "<<string(vals[j],'*')<<endl;//使用字符串展示结果
	}
}
//生成伯努利分布
void test8(){
	default_random_engine e;
	bernoulli_distribution b;//非模板类，默认p为0.5
	for(int i=0;i<15;++i){
		cout<<b(e)<<" ";
	}
	cout<<endl;
	bernoulli_distribution b2(.55);//55%的机会为true
	for(int i=0;i<15;++i){
		cout<<b2(e)<<" ";
	}
}

int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	//test6();
	//test7();
	test8();
}

