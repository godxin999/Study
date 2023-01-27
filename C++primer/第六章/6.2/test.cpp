/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/1/23 22:52:54
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//main:处理命令行选项
/*
int main(int argc,char** argv){
	cout<<"argc:"<<argc<<endl;
	while(argc-->0){
		printf("argc: %d,*argcv++: %s\n",argc,*argv++);
	}

}
PS C:\Users\admin\Desktop\Study\C++primer\第六章\6.2> ./a.exe 1 2 3
argc:4
argc: 3,*argcv++: C:\Users\admin\Desktop\Study\C++primer\第六章\6.2\a.exe
argc: 2,*argcv++: 1
argc: 1,*argcv++: 2
argc: 0,*argcv++: 3
*/
//省略符形参使用，如果首形参为int类型，系统调用第二个func函数
void func(...){
	cout<<"func1"<<endl;
}
void func(int i,...){
	cout<<"func2"<<endl;
}
void test1(){
	func(1,'a',"c");
	func(1,2,3);
	func("a","a",'a');
	func(3.14,"a",'1');
}
int main(){
	test1();
	return 0;
}