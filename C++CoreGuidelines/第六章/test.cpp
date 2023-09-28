/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/26 22:55:47
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>

//E.1 优先使用枚举而不是宏(宏没有类型，也没有作用域)
//E.2 使用枚举表示相关联的具名常量的合集
enum class Day{
	Mon,Tue,Wed,Thu,Fri,Sat,Sun
};
//E.3 优先使用enum class而不是不同enum
//有作用域枚举项(enum class)，不会自动转型为int，要访问它们则需要使用作用域运算符
//E.5 不要对枚举项使用全大写加下划线的命名方式，因为可能会与宏冲突
//E.6 避免使用无名枚举
//E.7 仅在有必要的时候指定枚举的底层类型(默认情况下，有作用域enum的类型为int)
//E.8 仅在有必要的时候指定枚举项的值
enum class Month{
	Jan=1,Feb,Mar,
	Apr,May,Jun,
	Jul,Aug,Sep,
	Oct,Nov,Dec
};
//枚举会在编译器检查其底层枚举项的值
/*
enum struct color:char{
	red=127,
	blue,
	green
};
*/

int main(){




	return 0;
}