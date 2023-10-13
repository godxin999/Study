/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/10/13 11:29:18
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>

//SF.1 如果项目还未采用其他约定，那么代码文件使用.cpp后缀，接口文件使用.h后缀
//SF.2 .h文件不可含有对象定义或非内联函数定义(ODR规则)
//SF.5 .cpp文件必须包含定义其接口的.h文件
//SF.8 为所有.h文件使用#include防护宏(名字唯一)
//SF.9 避免源文件之间的循环依赖
//SF.10 避免对隐含#include进来的名字的依赖
//比如GCC5.4在<iostream>中包含了<string>，但是依然需要#include<string>
//SF.11 头文件应该是自包含的，即头文件不依赖于之前包含的其他头文件

//如果名字是不需要解释的，如45min、300s，那么就使用using namespace，例如using namespace std::literals::chrono_literals;
//SF.7 不要再头文件的全局作用域中使用using namespace
//SF.20 使用namespace表示逻辑结构
//SF.21 不要在头文件中使用无名命名空间
//SF.22 为所有的内部/不导出的实体使用无名命名空间
//无名namespace使用内部链接，只能在当前翻译单元中使用，所以如果在头文件中使用无名命名空间，那么每个翻译单元都会有一个独立的实体，产生的可执行文件的大小会增加
//无名命名空间的用法类似C语言里使用的static关键字

int main(){





	return 0;
}