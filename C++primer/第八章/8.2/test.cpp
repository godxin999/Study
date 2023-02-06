/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/6 17:50:35
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <fstream>
using namespace std;
//使用文件流对象
void test1(){
	string s1="b.txt",s2="a.txt";
	ifstream in(s1);//提供文件名，open自动被调用
	ofstream out;
	in.close();//对一个已经打开的文件流调用open会失败，同时导致failbit置位，所以在关联到其他文件之前，需要关闭已经关联的文件
	in.open(s2);//打开其他文件
	//fstream对象在离开作用域之后会自动析构，close函数会被自动调用
}
//文件模式
void test2(){
	//以out模式打开的文件会丢弃已有数据(输出和截断)
	ofstream out("a.txt");//隐含以输出模式打开并截断文件
	ofstream out2("a.txt",ofstream::out);//隐含的截断文件
	ofstream out3("a.txt",ofstream::out|ofstream::trunc);
	//为了保留文件内容，需要显式指定app模式(输出和追加)
	ofstream app("a.txt",ofstream::app);//隐含为输出模式
	ofstream app2("a.txt",ofstream::out|ofstream::app);
}


int main(){
	//test1();
	test2();




}

