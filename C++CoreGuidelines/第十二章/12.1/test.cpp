/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/10/12 17:35:19
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>

//Con.1 默认情况下，使对象不可变
//Con.2 默认情况下，成员函数应声明为const
//物理常量性：对象被声明为const，因此不能被改变，他在内存中的表示方式是固定的
//逻辑常量性：对象被声明为const，但可以被改变，他的逻辑值是固定的，但是在内存中的表示方式可能在运行期发生变化
#include <mutex>
struct Immutable{
	mutable std::mutex m;//如果不声明为mutable，代码运行会发生错误
	int read()const{
		std::lock_guard<std::mutex> lock(m);
		//...
	}
};
//Con.3 默认情况下，传递指向const对象的指针或引用
void getCstring(const char* cStr);//传递的指针可能为空指针，需要在使用前检查
void getCppString(const std::string& cppStr);

//Con.4 使用const定义构造后值不可变的对象

int main(){





	return 0;
}