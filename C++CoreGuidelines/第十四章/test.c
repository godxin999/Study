/*************************************************************************
	> File Name: test.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/10/13 11:04:55
 ************************************************************************/

#include <stdio.h>

void test1(){
	printf("%d\n",sizeof('a'));//4，在C编译器中，字符常量是int类型
}

void test2(){
	char c='a';
	void* pv=&c;
	int* pi=pv;
}

int main(){
	//test1();
	test2();

	return 0;
}