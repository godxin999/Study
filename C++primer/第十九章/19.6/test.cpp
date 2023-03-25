/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/25 13:53:39
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//使用union
union Token{
	char cval;
	int ival;
	double dval;
};
void test1(){
	Token t1={'a'};//如果提供了初始值，那么其会用来初始化union的第一个对象，这里初始化cval成员
	Token t2;//未定义的Token对象
	Token *pt=new Token;//指向一个未初始化的Token对象的指针
	pt->ival=42;//赋值后其他数据成员会变成未定义的状态
}
//匿名union
void test2(){
	union {//所在作用域内该union的所有成员均可以直接访问
		char cval;
		int ival;
		double dval;
	};
	cval='c';
	ival=42;
}
/*
//在全局作用域要声明为static
static union {
	char cval;
	int ival;
	double dval;
};
*/
//使用类管理union成员
class token{
	friend ostream& operator<<(ostream&,const token&);
public:
	token():tok(INT),ival(0){}
	token(const token &t):tok(t.tok){
		copyUnion(t);
	}
	token(token &&t)noexcept:tok(t.tok){
		moveUnion(std::move(t));
	}
	token &operator=(const token &t){
		if(tok==STR&&t.tok!=STR){//如果待拷贝的值不是string
			sval.~string();//析构sval
		}
		if(tok==STR&&t.tok==STR){
			sval=t.sval;//直接赋值
		}
		else{
			copyUnion(t);//调用copyUnion函数进行拷贝
		}
		tok=t.tok;
		return *this;
	}
	token& operator=(token &&t)noexcept{
		if(this==&t)return *this;
		if(tok==STR&&t.tok!=STR){
			sval.~string();
		}
		if(tok==STR&&t.tok==STR){
			sval=std::move(t.sval);
		}
		else{
			moveUnion(std::move(t));
		}
		tok=t.tok;
		return *this;
	}
	~token(){
		if(tok==STR){//如果union存储的是string类型
			sval.~string();//显式调用string的析构函数
		}
	}
	token &operator=(const string &s){
		if(tok==STR){
			sval=s;
		}
		else{
			new(&sval) string(s);//利用定位new表达式创建一个string对象
		}
		tok=STR;
		return *this;
	}
	token &operator=(char c){
		if(tok==STR){
			sval.~string();
		}
		cval=c;
		tok=CHAR;
		return *this;
	}
	token &operator=(int i){
		if(tok==STR){
			sval.~string();
		}
		ival=i;
		tok=INT;
		return *this;
	}
	token &operator=(double d){
		if(tok==STR){
			sval.~string();
		}
		dval=d;
		tok=DBL;
		return *this;
	}
private:
	enum{INT,CHAR,DBL,STR} tok;
	union{
		char cval;
		int ival;
		double dval;
		string sval;
	};
	void copyUnion(const token &t){
		switch (t.tok){//检验判别式来拷贝具体的值
			case token::INT:{
				ival=t.ival;
				break;
			}
			case token::CHAR:{
				cval=t.cval;
				break;
			}
			case token::DBL:{
				dval=t.dval;
				break;
			}
			case token::STR:{
				new(&sval) string(t.sval);
				break;
			}
		}
	}
	void moveUnion(token &&t){
		switch (t.tok){
			case INT:{
				ival=t.ival;
				break;
			}
			case CHAR:{
				cval=t.cval;
				break;
			}
			case DBL:{
				dval=t.dval;
				break;
			}
			case STR:{
				new(&sval) string(std::move(t.sval));
				break;
			}
		}
	}
};
inline
ostream& operator<<(ostream &os,const token &t){
	switch (t.tok){//检验判别式来拷贝具体的值
		case token::INT:{
			return os<<t.ival;
		}
		case token::CHAR:{
			return os<<t.cval;
		}
		case token::DBL:{
			return os<<t.dval;
		}
		case token::STR:{
			return os<<t.sval;
		}
	}
}
void test3(){
	token t1,t2;
	t1='c';
	t2="ssss";
	cout<<t1<<endl;//c
	cout<<t2<<endl;//ssss
	t1=t2;
	t1=42;
	cout<<t1<<endl;//42;
	t1=std::move(t2);
	cout<<t1<<endl;//ssss
	token t3(std::move(t1));
	cout<<t3<<endl;//ssss
}
int main(){
	//test1();
	//test2();
	test3();
}