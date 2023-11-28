/*************************************************************************
	> File Name: aligned_allocator.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/11/28 9:16:35
 ************************************************************************/

#include <iostream>
#include <cstdint>
#include <cassert>

uintptr_t AlignAddress(uintptr_t address,size_t alignment){
	const size_t mask=alignment-1;//alignment为2的倍数，-1为掩码
	assert((alignment&mask)==0);
	return (address+mask)&~mask;//返回对齐后的地址
}

template <typename T>
T* AlignPointer(T* pointer,size_t alignment){
	//因为要在对齐后的地址定位构造，所以T类型的大小需要小于alignment
	const uintptr_t address=reinterpret_cast<uintptr_t>(pointer);
	const uintptr_t aligned_address=AlignAddress(address,alignment);
	return reinterpret_cast<T*>(aligned_address);
}
/*
void* AllocAligned(size_t bytes,size_t alignment){
	size_t worstCaseBytes=bytes+alignment-1;//因为至少分配一个字节，bytes+alignment-1可以保证至少有alignment个字节(可以对齐)
	unsigned char* p=new unsigned char[worstCaseBytes];
	return AlignPointer(p,alignment);//但是无法释放
}
*/

void* AllocAligned(size_t bytes,size_t alignment){
	size_t actualBytes=bytes+alignment;//至少多出一个字节，至多多出alignment个字节
	unsigned char* p=new unsigned char[actualBytes];
	//std::cout<<(void*)p<<std::endl;
	unsigned char* aligned_p=AlignPointer(p,alignment);//获取对齐后的指针
	//std::cout<<(void*)aligned_p<<std::endl;
	if(aligned_p==p){//如果指针已经对齐，那么往后移动alignment个字节(对齐到下一个边界)
		aligned_p+=alignment;
	}
	ptrdiff_t offset=aligned_p-p;//计算偏移量
	assert(offset>0&&offset<=256);//偏移量必须在0到256之间
	aligned_p[-1]=static_cast<unsigned char>(offset&0xff);//将偏移量存储在对齐后的指针的前一个字节
	return aligned_p;
}

void FreeAligned(void* p){
	if(!p) return;
	unsigned char* aligned_p=reinterpret_cast<unsigned char*>(p);
	ptrdiff_t offset=aligned_p[-1];//获取偏移量
	if(offset==0) offset=256;
	std::cout<<"offset:"<<offset<<std::endl;
	unsigned char* unaligned_p=aligned_p-offset;//获取原始指针
	delete[] unaligned_p;//释放原始指针
}

#include <stack>

int main(){
	/*
	unsigned char* p=reinterpret_cast<unsigned char*>(AllocAligned(155,256));
	ptrdiff_t offset=p[-1];
	FreeAligned(p);
	*/
	std::stack<unsigned char*> s;
	for(int i=0;i<=256;++i){
		unsigned char* p=reinterpret_cast<unsigned char*>(AllocAligned(i,256));
		s.push(p);
	}
	for(int i=0;i<=256;++i){
		unsigned char* p=s.top();
		s.pop();
		FreeAligned(p);
	}

	return 0;
}