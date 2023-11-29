/*************************************************************************
	> File Name: single_frame_double_buffered_allocator.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/11/29 12:14:46
 ************************************************************************/

#include "stack_allocator.hpp"
#include <iostream>


void test_single_frame_allocator(){
	StackAllocator g_singleFrameAllocator;
	
	for(int i=0;i<100;++i){
		g_singleFrameAllocator.clear();
		auto marker=g_singleFrameAllocator.getMarker();
		std::cout<<"marker:"<<marker<<std::endl;
		void* mem=g_singleFrameAllocator.alloc(100);
		marker=g_singleFrameAllocator.getMarker();
		std::cout<<"marker:"<<marker<<std::endl;
	}
}

class DoubleBufferedStackAllocator{
private:
	unsigned m_currentFrameIndex;
	StackAllocator m_frameAllocators[2];
public:
	void swapBuffer(){
		m_currentFrameIndex^=1;
	}
	void clearCurrentBuffer(){
		m_frameAllocators[m_currentFrameIndex].clear();
	}
	void* alloc(unsigned size_bytes){
		return m_frameAllocators[m_currentFrameIndex].alloc(size_bytes);
	}

};

void test_double_buffered_allocator(){
	DoubleBufferedStackAllocator g_doubleBufferedAllocator;
	for(int i=0;i<100;++i){
		//g_singleFrameAllocator.clear();
		//第n帧的数据可以在第n+1帧使用
		g_doubleBufferedAllocator.swapBuffer();
		//清除n-1帧的数据
		g_doubleBufferedAllocator.clearCurrentBuffer();
		void* mem=g_doubleBufferedAllocator.alloc(100);

	}
}

int main(){
	//test_single_frame_allocator();
	test_double_buffered_allocator();


	return 0;
}