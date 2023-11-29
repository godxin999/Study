/*************************************************************************
	> File Name: stack_allocator.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/11/26 10:22:22
 ************************************************************************/

#include <iostream>
#include <cassert>

class StackAllocator{
public:
	//Marker代表栈顶的位置
	using Marker=unsigned;
	//构造函数
	explicit StackAllocator(unsigned stackSize_bytes=4096){
		//分配内存
		base=new unsigned char[stackSize_bytes];
		stack_size=stackSize_bytes;
		marker=0;
	}
	//从栈顶分配给定大小的新内存块
	void* alloc(unsigned size_bytes){
		//检查是否有足够的空间
		assert(marker+size_bytes<=stack_size);
		//计算新内存块的地址
		void* mem=base+marker;
		//更新栈顶
		marker+=size_bytes;
		return mem;
	}
	//返回当前栈顶的标记
	Marker getMarker(){
		return marker;
	}
	//将栈回滚至之前的标记
	void freeToMarker(Marker pre_marker){
		assert(pre_marker<=marker);
		marker=pre_marker;
	}
	//清空整个栈
	void clear(){
		marker=0;
	}
	//析构函数
	~StackAllocator(){
		delete[] base;
	}
private:
	unsigned char* base;
	unsigned stack_size;
	Marker marker;
};

class DoubleEndedStackAllocator{
public:
	using Marker=unsigned;
	explicit DoubleEndedStackAllocator(unsigned stackSize_bytes){
		base=new unsigned char[stackSize_bytes];
		stack_size=stackSize_bytes;
		lower_marker=0;
		upper_marker=stack_size;
	}
	void* allocLower(unsigned size_bytes){
		assert(lower_marker+size_bytes<=upper_marker);
		void* mem=base+lower_marker;
		lower_marker+=size_bytes;
		return mem;
	}
	void* allocUpper(unsigned size_bytes){
		assert(upper_marker-size_bytes>=lower_marker);
		upper_marker-=size_bytes;
		void* mem=base+upper_marker;
		return mem;
	}
	Marker getLowerMarker(){
		return lower_marker;
	}
	Marker getUpperMarker(){
		return upper_marker;
	}
	void freeToLowerMarker(Marker pre_marker){
		assert(pre_marker<=lower_marker);
		lower_marker=pre_marker;
	}
	void freeToUpperMarker(Marker pre_marker){
		assert(pre_marker>=upper_marker);
		upper_marker=pre_marker;
	}
	void clear(){
		lower_marker=0;
		upper_marker=stack_size;
	}
	~DoubleEndedStackAllocator(){
		delete[] base;
	}
private:
	unsigned char* base;
	unsigned stack_size;
	Marker lower_marker;
	Marker upper_marker;
};

/*

int main(){
	StackAllocator stack(1024);
	auto *a=stack.alloc(sizeof(int));
	auto *b=stack.alloc(sizeof(long long));
	auto *c=stack.alloc(sizeof(char));
	std::cout<<a<<" "<<b<<" "<<c<<std::endl;
	
	DoubleEndedStackAllocator double_stack(1024);
	auto *d=double_stack.allocLower(sizeof(int));
	auto *e=double_stack.allocUpper(sizeof(char));
	auto *f=double_stack.allocLower(sizeof(char));
	auto *g=double_stack.allocUpper(sizeof(long long));
	std::cout<<d<<" "<<e<<" "<<f<<" "<<g<<std::endl;
	

	return 0;
}

*/