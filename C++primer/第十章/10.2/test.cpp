/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/20 11:03:19
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <numeric>
#include <vector>
#include <list>
using namespace std;
//只读算法accumulate
void test1(){
	vector<int> v={1,2,3,4,5,6,7,8,9};
	int sum=accumulate(v.cbegin(),v.cend(),0);
	cout<<sum<<endl;
	vector<string> sv={"a","b","c","d","e","f"};
	//string ans1=accumulate(sv.cbegin(),sv.cend(),"");//错误，const char*没有实现重载加法
	string ans2=accumulate(sv.cbegin(),sv.cend(),string(""));
	cout<<ans2<<endl;//abcdef
}
//只读算法equal
void test2(){
	//equal要求第二个序列的元素至少与第一个序列相等，且两种元素类型要可以使用==进行比较，这里比较的是string和const char*
	vector<string> sv={"a","b","c"};
	list<const char*> ccv={"a","b","c"};
	cout<<equal(sv.cbegin(),sv.cend(),ccv.cbegin());//1
}
//fill和fill_n写入容器
void test3(){
	vector<int> v={0,1,2,3,4,5,6,7,8,9};
	fill(v.begin(),v.end(),0);//fill接受两个迭代器和元素值，并在迭代器范围内进行写入操作，这里全部赋值为0
	for(auto i:v){
		cout<<i<<endl;
	}
	fill(v.begin(),v.begin()+v.size()/2,10);//前半个序列设置为10
	for(auto i:v){
		cout<<i<<endl;
	}
	fill_n(v.begin(),10,100);//fill_n接受单迭代器、元素个数和元素值，但是要求容器大小足够
	for(auto i:v){
		cout<<i<<endl;
	}
}
//插入迭代器
void test4(){
	vector<int> v;
	auto it=back_inserter(v);//通过插入迭代器进行赋值会把元素插入到容器中
	*it=42;
	for(auto i:v){
		cout<<i<<endl;
	}
	fill_n(back_inserter(v),10,0);//向v中插入10个零
	for(auto i:v){
		cout<<i<<endl;
	}
}
//拷贝算法
void test5(){
	int a1[]={0,1,2,3,4,5,6,7,8,9};
	int a2[sizeof(a1)/sizeof(*a1)];
	//int a2[end(a1)-begin(a1)];//等价的定义
	auto ret=copy(begin(a1),end(a1),a2);//ret指向a2的尾后位置
	for(auto i:a2){
		cout<<i<<endl;
	}
}
//replace和replace_copy
void test6(){
	vector<int> v={0,1,0,3,0,5,6,0,8,9};
	replace(v.begin(),v.end(),0,100);//把v中所有的0替换成100
	/*for(auto i:v){
		cout<<i<<endl;
	}*/
	vector<int> v2;
	replace_copy(v.cbegin(),v.cend(),back_inserter(v2),100,0);//调用后v不会改变，v2中存有改动后的序列
	for(auto i:v){
		cout<<i<<endl;
	}
	for(auto i:v2){
		cout<<i<<endl;
	}
}
//排序算法去重
void test7(){
	vector<int> v={0,1,2,3,2,1,5,6,2,3,5,4,5,2,4,6,7,8,3,3,5,4,2,6,3,4,4,5,5};
	sort(v.begin(),v.end());
	auto end_unique=unique(v.begin(),v.end());
	v.erase(end_unique,v.end());
	for(auto i:v){
		cout<<i<<endl;
	}
}

int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	//test6();
	test7();
}

