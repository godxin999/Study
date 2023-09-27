/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/26 21:16:22
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>


//联合体模板(被视为一种类模板)
template <typename T>
union AllocChunk{
	T object;
	unsigned char bytes[sizeof(T)];
};

//默认调用参数
template <typename T>
void func(std::vector<T> &v,int num=10){
	//如果传入了第二个参数，那么默认参数不会被实例化
}
//类模板的非模板成员
template <typename I>
class CupBoard{
	class Shelf;
	void open();
	enum Wood:unsigned char;
	//static double totalWeight;
	//C++17之后，静态成员可以在类模板内部使用inline关键字初始化
	inline static double totalWeight = 0.0;
};
template <typename I>
class CupBoard<I>::Shelf{
	//...
};
template <typename I>
void CupBoard<I>::open(){
	//...
}
template <typename I>
enum CupBoard<I>::Wood:unsigned char{
	OAK,
	MAHOGANY,
	PLYWOOD
};

//类模板无法和不同类型的实体共享名称
int C;
class C{};
int X;
//template<typename T>
//class X{};

//模板通常具有外部链接，一些例外为：
//命名空间作用域中具有静态限定符的函数模板，匿名空间的直接或间接的成员的模板，它们具有内部链接
//匿名类的成员模板，它们没有链接


int main(){





	return 0;
}