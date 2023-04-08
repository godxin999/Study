/*************************************************************************
	> File Name: item11.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/8 16:51:34
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//istream类和ostream类不能被拷贝，其拷贝相关函数在C++98中声明为私有的
template <class charT,class traits=char_traits<charT> >
class basic_ios:public ios_base{
public:
	//...
private:
	basic_ios(const basic_ios&);
	basic_ios& operator=(const basic_ios&);
};
//而在C++11中，我们可以将其定义为删除的函数，这时需要将待删除的函数声明为public的
template <class charT,class traits=char_traits<charT> >
class basic_ios:public ios_base{
public:
	basic_ios(const basic_ios&)=delete;
	basic_ios& operator=(const basic_ios&)=delete;
};
//任何函数都可以标记为delete的
bool isLucky(int number){cout<<"isLucky(int)"<<endl;return true;}
bool isLucky(char)=delete;
bool isLucky(bool)=delete;
bool isLucky(double)=delete;
void test1(){
	isLucky(1);
	//isLucky('a');//错误，无法调用删除的函数
	//isLucky(true);
	//isLucky(2.3);
}
//delete还可以禁止一些模板的实例化
template <typename T>
void processPointer(T* ptr){}
template <>//进行特例化，禁止使用void*或char* 调用模板
void processPointer<void>(void*)=delete;
template <>
void processPointer<char>(char*)=delete;
//按常理包含cv限定符的版本也需要删除，此外还需要删除标准字符类型的重载版本，比如wchar_t、char16_t、char32_t
template <>
void processPointer<const void>(const void*)=delete;
template <>
void processPointer<const char>(const char*)=delete;
//类中的函数模板可以在类外声明特例化为删除的
class widget{
public:
	template <typename T>
	void f(T* ptr){}
};
template <>//模板特例化需要在命名空间作用域内，而不是类作用域内，所以C++98声明私有的方式无法禁用接受void*的函数模板
void widget::f<void>(void*)=delete;

int main(){
	test1();
}