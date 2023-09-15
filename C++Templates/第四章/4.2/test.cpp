/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/15 21:48:01
 ************************************************************************/

#include <iostream>

template <typename... T>
auto foldsum(T... s){
	return(...+s);
}

struct Node{
	int value;
	Node* left;
	Node* right;
	Node(int i=0):value(i),left(nullptr),right(nullptr){}
};

auto Left = &Node::left;
auto Right = &Node::right;

//折叠表达式展开，返回路径上的最后一个节点
template <typename T,typename... Path>
Node* traverse(T np,Path... path){
	return (np->*...->*path);
}

void test1(){
	Node* root=new Node{0};
	root->left=new Node{1};
	root->left->right=new Node{2};
	
	traverse(root,Left,Right);
}

//通过折叠表达式打印可变参数模板
template <typename... T>
void print(const T&... args){
	(std::cout<<...<<args)<<std::endl;
}

void test2(){
	print(1,2,3,4,5);
}

//带有空格的打印
template <typename T>
class addSpace{
private:
	const T& ref;
public:
	addSpace(const T& r):ref(r){}
	//或许使用sstream更好?
	friend std::ostream& operator<<(std::ostream& os,const addSpace<T>& s){
		return os<<s.ref<<' ';
	}
};

template <typename... Args>
void printWithSpace(Args... args){
	(std::cout<<...<<addSpace<Args>(args))<<std::endl;
}

void test3(){
	printWithSpace(1,2,3,4,5);
}

int main(){
	//test1();
	//test2();
	test3();


	return 0;
}