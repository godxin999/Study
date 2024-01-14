/*************************************************************************
	> File Name: 9.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/14 21:33:11
 ************************************************************************/

#include <iostream>

//C++20指定初始化
struct Point{
	int x=100;
	int y=100;
	int z=100;
};
struct Line{
	Point p1;
	Point p2;
};
//1.要求对象是一个聚合类型
//2.非静态数据类型成员的初始化要按声明的顺序
//3.不能混用指定初始化和列表初始化
void test1(){
	Point p{.x=1,.y=2,.z=3};
	std::cout << p.x << std::endl;//1
	std::cout << p.y << std::endl;//2
	std::cout << p.z << std::endl;//3
	Point p1{.z=3};
	std::cout << p1.x << std::endl;//100
	std::cout << p1.y << std::endl;//100
	std::cout << p1.z << std::endl;//3
}
//4.不能嵌套指定初始化成员
void test2(){
	//Line l1{.p1.x=1};//编译错误
	//使用另一种方式初始化
	Line l{.p1{.x=1,.y=2,.z=3},.p2{.x=4,.y=5,.z=6}};
	std::cout << l.p1.x << std::endl;//1
	std::cout << l.p1.y << std::endl;//2
	std::cout << l.p1.z << std::endl;//3
	std::cout << l.p2.x << std::endl;//4
	std::cout << l.p2.y << std::endl;//5
	std::cout << l.p2.z << std::endl;//6
}
//5.联合体内的数据成员只能初始化一次
union U{
	int x;
	int y;
};
void test3(){
	//U u1{.x=1,.y=2};//编译错误
	U u{.x=1};
	std::cout << u.x << std::endl;//1
	std::cout << u.y << std::endl;//1
}
int main(){
	//test1();
	//test2();
	test3();


	return 0;
}