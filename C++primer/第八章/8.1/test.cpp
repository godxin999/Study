/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/6 13:37:30
 ************************************************************************/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
using namespace std;
//IO对象无拷贝或赋值
void test1(){
	ofstream out1,out2;
	//out1=out2;//错误，不能对流对象赋值
	//ofstream print(ofstream);//不能初始化ofstream对象
	//out2=print(out2);//不能拷贝流对象
}
//管理条件状态
void test2(){
	auto old_state=cin.rdstate();//记录cin的当前状态
	cin.clear();//使cin有效(复位所有flag)
	//使用cin
	cin.setstate(old_state);//将cin置为原有状态
}
//复位某个标志位
void test3(){
	//eofbit:001
	//failbit:010
	//badbit:100
	//goodbit:000
	cout<<cin.rdstate()<<endl;//0
	int a;cin>>a;//输入一个字符，failbit置位
	cout<<cin.rdstate()<<endl;//2
	//cin.clear(cin.goodbit);//以goodbit为参数复位
	//cout<<cin.rdstate()<<endl;//0
	cin.clear(cin.rdstate()&~cin.failbit);//复位
	cout<<cin.rdstate()<<endl;//0
}
//刷新缓冲区
void test4(){
	cout<<"hi!"<<endl;//刷新缓冲区并换行
	cout<<"hi!"<<flush;//刷新缓冲区
	cout<<"hi!"<<ends;//刷新缓冲区并添加一个'\0'字符，windows平台下显示为一个空格
}
//unitbuf操纵符
void test5(){
	cout<<unitbuf;//所有输出操作后都会立即刷新缓冲区(无缓冲)
	cout<<nounitbuf;//回到正常的缓冲区刷新机制
}
//关联输入和输出流
void test6(){
	cin.tie(&cout);//cin关联到cout
	ostream *old_tie=cin.tie(nullptr);//old_tie指向当前关联到cin的输出流(如果有)，然后解除cin的关联
	cin.tie(old_tie);//恢复和cout之间的关联
}
int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	test6();

}

