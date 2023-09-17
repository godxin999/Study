/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/17 10:34:22
 ************************************************************************/

#include <iostream>
#include <vector>

template <typename T>
void printcoll(const T& coll){
	//使用typename表示为一个类型
	//为了使用模板类型T的const_iterator，必须使用typename
	typename T::const_iterator pos;
	typename T::const_iterator end(coll.end());
	for(pos=coll.begin();pos!=end;++pos){
		std::cout<<*pos<<" ";
	}
	std::cout<<std::endl;
}

int main(){
	std::vector<int> v={1,2,3,4,5};
	printcoll(v);
	return 0;
}