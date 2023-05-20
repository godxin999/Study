/*************************************************************************
	> File Name: item34.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/20 10:47:27
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <functional>
#include <chrono>
using namespace std;
//使用lambda而不是std::bind
using namespace std::chrono;
using namespace std::literals;
using namespace std::placeholders;
using Time=std::chrono::steady_clock::time_point;
enum class Sound{
	Beep,Siren,Whistle
};
using Duration=std::chrono::steady_clock::duration;
void setAlarm(Time t,Sound s,Duration d){}
void test1(){
	auto setSoundL=[](Sound s){
		setAlarm(steady_clock::now()+1h,s,30s);
	};
	setSoundL(Sound::Beep);

	auto setSoundB=std::bind(setAlarm,std::bind(std::plus<>(),steady_clock::now(),1h),_1,30s);
	setSoundB(Sound::Beep);
}
enum class Volume{
	Normal,Loud,Loudplusplus
};
void setAlarm(Time t,Sound s,Duration d,Volume v){}
void test2(){
	/*
	auto setSoundB=std::bind(setAlarm,std::bind(std::plus<>(),steady_clock::now(),1h),_1,30s);//错误，setAlarm存在重载
	*/
	using SetAlarmParamType=void(*)(Time,Sound,Duration);
	auto setSoundB=std::bind(static_cast<SetAlarmParamType>(setAlarm),std::bind(std::plus<>(),steady_clock::now(),1h),_1,30s);
	setSoundB(Sound::Beep);
}
void test3(){
	int hval=22;
	int lval=4;
	auto betweenL=[lval,hval](const auto &val){
		return lval<=val&&val<=hval;
	};
	cout<<boolalpha<<betweenL(10)<<endl;

	auto betweenB=std::bind(std::logical_and<>(),std::bind(std::less_equal<>(),lval,_1),std::bind(std::less_equal<>(),_1,hval));
	cout<<betweenB(10)<<endl;
}
class foo{
public:
	template <typename T>
	void operator()(const T& param)const{
		cout<<param<<endl;
	}
};
void test4(){
	foo f;
	auto boundf=std::bind(f,_1);
	boundf(100);
	boundf("fasfsd");

	auto boundfL=[f](const auto &param){
		f(param);
	};
	boundfL(1000);
	boundfL("dsadsaa");
}


int main(){
	//test1();
	//test2();
	//test3();
	test4();


}