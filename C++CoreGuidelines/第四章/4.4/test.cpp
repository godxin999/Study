/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/19 13:46:14
 ************************************************************************/

#include <iostream>
#include <cstring>


//F.42 返回T*仅用于表示位置
struct Node{
	std::string name;
	int num;
	Node* left;
	Node* right;
	Node(const std::string& s,int n=0,Node* l=nullptr,Node* r=nullptr):name(s),num(n),left(l),right(r){}
};
Node* find(Node* t,const std::string& s){
	if(!t||t->name==s)return t;
	if(auto p=find(t->left,s); p) return p;
	if(auto p=find(t->right,s); p) return p;	
	return nullptr;
}
void test1(){
	Node* root=new Node("root",0);
	Node* left=new Node("left",1);
	Node* right=new Node("right",2);
	Node* left_left=new Node("left_left",11);
	Node* left_right=new Node("left_right",12);
	Node* right_left=new Node("right_left",21);
	Node* right_right=new Node("right_right",22);
	root->left=left;
	root->right=right;
	left->left=left_left;
	left->right=left_right;
	right->left=right_left;
	right->right=right_right;
	std::cout<<find(root,"left_left")->num<<std::endl;
}

//F.44 当不希望发生拷贝，也不需要表达没有返回对象时，应该返回T&
//典型例子为输入和输出流或赋值运算符
//A& operator=(const A& rhs)可以避免链式操作时对于不必要的临时对象的拷贝

//F.45 不要返回T&&
//F.48 不要返回std::move(x)
//因为存在RVO和NRVO优化，如果进行上述操作则会导致程序变慢

//F.46 main()的返回类型为int

int main(){
	//test1();
	return 0;
}