/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/25 16:55:56
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <memory>
#include <vector>
using namespace std;
//new分配动态数组
void test1(){
	//typedef int arrT[42];
	using arrT=int[42];
	int *p=new arrT;//分配42个int并返回第一个int的指针
}
//初始化动态数组
void test2(){
	int *pi1=new int[10]();//初始化为10个0
	//int *pi2=new int[10](100);//错误，只能用空括号进行值初始化，不能提供初始化器，也意味着不能用auto分配动态数组
	string *ps1=new string[10];
	string *ps2=new string[10]();
	int *pi2=new int[10]{0,1,2,3,4,5,6,7};//使用列表中的初始化器分别进行初始化，不足的元素将会进行值初始化
	for(int i=0;i<10;i++){
		cout<<pi2[i]<<" ";//0 1 2 3 4 5 6 7 0 0
	}
	cout<<endl;
}
//动态分配空数组
void test3(){
	char arr[0];//可以编译通过，但是错误，不能定义长度为0的数组
	char *cp=new char[0];//正确，但是cp不能进行解引用
}
//delete动态数组
void test4(){
	using arrT=int[42];
	int *p=new arrT;
	delete []p;//释放动态数组时必须加上方括号
}
//智能指针管理动态数组
void test5(){
	//要用unique_ptr管理动态数组就需要在对象类型后面加上一对空方括号
	unique_ptr<int[]> up(new int[10]);//up指向一个包含10个未初始化int的数组
	//up.release();//自动用delete销毁其指针
	for(auto i=0;i!=10;++i){
		up[i]=i;//当unique_ptr指向数组时，我们可以使用下标运算符来访问数组中的元素
		cout<<up[i]<<" ";
	}
	cout<<endl;
}
//使用shared_ptr管理动态数组
void test6(){
	shared_ptr<int> sp(new int[10],[](int *p){//使用lambda表达式提供自定义删除器
		delete[] p;
	});
	for(auto i=0;i!=10;++i){
		*(sp.get()+i)=i;//shared_ptr不支持使用下标运算符，所以只能获取裸指针来对数组进行访问
		cout<<*(sp.get()+i)<<" ";
	}
	cout<<endl;
}
//allocator
void test7(){
	allocator<string> alloc;//可以分配string的allocator对象
	auto const p=alloc.allocate(10);//分配10个未初始化的string
	auto q=p;//q指向最后构造的元素之后的位置
	alloc.construct(q++);//*q为空字符串
	alloc.construct(q++,10,'c');//*q为cccccccccc
	alloc.construct(q++,"hi");//*q为hi
	cout<<*p<<endl;
	cout<<*(p+1)<<endl;
	cout<<*(p+2)<<endl;
	cout<<*q<<endl;//q指向未构造的内存
	while(q!=p){
		alloc.destroy(--q);//释放真正构造的string
	}
	alloc.deallocate(p,10);//参数大小必须和allocate时提供的大小相同
}
//拷贝和填充未初始化内存
void test8(){
	vector<int> v(10);
	allocator<int> alloc;
	auto p=alloc.allocate(v.size()*2);//分配v二倍的内存
	auto q=uninitialized_copy(v.begin(),v.end(),p);//拷贝v中的元素来构造从p开始的元素，并返回一个指针指向最后一个构造的元素之后的位置
	uninitialized_fill_n(q,v.size(),42);//将剩余的元素初始化为42
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

