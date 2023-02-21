/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/20 17:15:37
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <functional>
using namespace std;
bool isShorter(const string &s1,const string &s2){
	return s1.size()<s2.size();
}
void test1(){
	vector<string> words={"apple","banana","peach","pear","berry","watermelon","orange","lemon","mango"};
	sort(words.begin(),words.end());//按字典排序
	for(auto s:words){
		cout<<s<<endl;
	}
	stable_sort(words.begin(),words.end(),isShorter);//稳定排序以长短为谓词的同时保证相同长度的单词以字典序排列(相对位置不变)
	for(const auto &s:words){
		cout<<s<<endl;
	}
}
//lambda表达式
void test2(){
	auto f=[]{return 42;};//lambda表达式必须包含捕获列表和函数体
	cout<<f()<<endl;
	cout<<endl;
	vector<string> words={"apple","banana","peach","pear","berry","watermelon","orange","lemon","mango"};
	sort(words.begin(),words.end());//先按字典序排序
	stable_sort(words.begin(),words.end(),[](const string &s1,const string &s2){return s1.size()<s2.size();});//利用lambda表达式代替isShorter
	for(auto i:words){
		cout<<i<<endl;
	}
}
//使用捕获列表
void test3(){
	vector<string> words={"apple","banana","peach","pear","berry","watermelon","orange","lemon","mango"};
	string s="seven";
	auto sz=s.size();
	auto wc=find_if(words.begin(),words.end(),[sz](const string &a){
		return a.size()>sz;
	});//找到words中第一个长度大于sz的位置
	if(wc!=words.end())cout<<*wc<<endl;//find_if返回一个迭代器
}
//for_each算法
void test4(){
	vector<int> v={0,1,2,3,4,5,6,7,8,9};
	for_each(v.begin(),v.end(),[](const int &a){cout<<a<<" ";});//for_each接受一个可调用对象，并对迭代器范围内的元素调用这个对象
	cout<<endl;
}
//值捕获
void test5(){
	int i=100;
	auto f=[i]{return i;};//值捕获在lambda创建时拷贝，而不是调用时拷贝
	i=0;
	cout<<f()<<endl;//100，f保存的是创建f时对i的拷贝
}
//引用捕获
void test6(){
	int i=100;
	auto f=[&i]{return i;};//调用lambda表达式时，使用的是引用绑定的对象
	i=0;
	cout<<f()<<endl;//0
	vector<int> v={0,1,2,3,4,5,6,7,8,9};
	ostream &os=cout;
	char c=' ';
	for_each(v.begin(),v.end(),[&os,c](const int &a){//ostream对象不能进行拷贝，只能进行引用捕获
		os<<a<<c;//0 1 2 3 4 5 6 7 8 9
	});
	cout<<endl;
}
//隐式捕获
void test7(){
	vector<string> words={"apple","banana","peach","pear","berry","watermelon","orange","lemon","mango"};
	string s="seven";
	auto sz=s.size();
	auto wc=find_if(words.begin(),words.end(),[=](const string &a){
		return a.size()>sz;
	});//使用隐式捕获且为值捕获
	if(wc!=words.end())cout<<*wc<<endl;

	vector<int> v={0,1,2,3,4,5,6,7,8,9};
	ostream &os=cout;
	char c=' ';
	for_each(v.begin(),v.end(),[&,c](const int &a){//混合使用显式捕获和隐式捕获，如果隐式捕获使用引用捕获，那么显式捕获就要使用值捕获，反之同理
		os<<a<<c;//0 1 2 3 4 5 6 7 8 9
	});
	cout<<endl;
	for_each(v.begin(),v.end(),[=,&os](const int &a){
		os<<a<<c;//0 1 2 3 4 5 6 7 8 9
	});
	cout<<endl;	
}
//可变lambda
void test8(){
	int i=40;
	auto f=[i]()mutable{//加入mutable关键字，就可以改变通过值捕获拷贝的变量的值
		return ++i;
	};
	i=0;
	cout<<f()<<endl;//41
}
//指定lambda返回类型
void test9(){
	vector<int> v={1,-2,3,-4,5,-6,7,-8,9};
	//transform接受四个参数，前两个为范围，第三个为写入的目的位置，第四个参数为一个可调用对象，这里把v中的负数替换为其绝对值
	/*
	transform(v.begin(),v.end(),v.begin(),[](int i){
		return i<0?-i:i;
	});
	for(auto i:v){
		cout<<i<<endl;
	}
	*/
	/*
	transform(v.begin(),v.end(),v.begin(),[](int i){
		if(i<0)return -i;
		else return i;
	});
	for(auto i:v){
		cout<<i<<endl;
	}
	*/
	//使用尾置返回类型来指定lambda返回类型
	transform(v.begin(),v.end(),v.begin(),[](int i)->int{
		if(i<0)return -i;
		else return i;
	});
	for(auto i:v){
		cout<<i<<endl;
	}
}
//参数绑定
bool check_size(const string &s,string::size_type sz){
	return s.size()>sz;
}
void test10(){
	vector<string> words={"apple","banana","peach","pear","berry","watermelon","orange","lemon","mango"};
	string s="seven";
	//find_if接受一个一元谓词，所以传入的可调用对象必须接受单一参数，我们可以使用bind来生成一个可调用对象，并把第二个参数绑定到sz，第一个参数用占位符_1替代，这些占位符定义在std::placeholders中，可以通过声明命名空间以省略对每个占位符的using声明
	using namespace std::placeholders;
	auto wc=find_if(words.begin(),words.end(),bind(check_size,_1,s.size()));
	cout<<*wc<<endl;
}
//通过bind重新安排参数顺序
void f(int a,int b,int c,int d,int e){
	cout<<a<<" "<<b<<" "<<c<<" "<<d<<" "<<e<<endl;
};
void test11(){
	using namespace std::placeholders;
	auto g=bind(f,100,200,_2,300,_1); //g的第一个参数对应f的第五个参数，第二个参数对应f的第三个参数
	g(400,500); //100 200 500 300 400
	vector<string> words={"apple","banana","peach","pear","berry","watermelon","orange","lemon","mango"};
	sort(words.begin(),words.end(),isShorter); //按从短到长排序
	for(auto i:words){
		cout<<i<<endl;
	}
	cout<<endl;
	sort(words.begin(),words.end(),bind(isShorter,_2,_1)); //调换参数位置，按从长到短排序
	for(auto i:words){
		cout<<i<<endl;
	}
}
//bind绑定引用参数
ostream &print(ostream &os,const int &i,char c){
	return os<<i<<c;
}
void test12(){
	vector<int> v={0,1,2,3,4,5,6,7,8,9};
	using namespace std::placeholders;
	ostream &os=cout;
	/*
	char c=' ';
	for_each(v.begin(),v.end(),[&os,c](const int &i){
		os<<i<<c;
	});
	cout<<endl;
	*/
	for_each(v.begin(),v.end(),bind(print,ref(os),_1,' ')); //通过bind绑定引用参数替换了一个引用捕获的lambda表达式，其中ref函数会返回一个对象，包含给定的引用，cref函数返回const引用
}

int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	//test6();
	//test7();
	//test8();
	//test9();
	//test10();
	//test11();
	test12();
}

