/*************************************************************************
	> File Name: 30.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2024/1/21 10:46:54
 ************************************************************************/

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <type_traits>
//alignof
void test1(){
    std::cout<<alignof(int)<<std::endl;//4
    std::cout<<alignof(std::max_align_t)<<std::endl;//8
    for(int i=0;i<100;++i){
        auto *p=new char;
        auto addr=reinterpret_cast<uintptr_t>(p);
        //即使申请一字节内存，内存分配器也会将指针定位到与std::max_align_t对齐的地址上
        std::cout<<addr%alignof(std::max_align_t)<<std::endl;//0
        delete p;
    }
}
//alignas
struct alignas(16) A{
    char c;
    int i;
    double d;
};
struct B{
    alignas(16) char a1;
    alignas(double) int a2;
    double a3;
};
void test2(){
    std::cout<<alignof(A)<<std::endl;//16
    std::cout<<alignof(B)<<std::endl;//16
}
//std::alignment_of，获取类型的对其字节长度
void test3(){
    std::cout<<std::alignment_of_v<int><<std::endl;//4
    std::cout<<std::alignment_of_v<std::max_align_t><<std::endl;//8
    std::cout<<std::alignment_of<std::max_align_t>()<<std::endl;//8
}
//std::aligned_storage，获取指定长度和对齐方式的内存块
void test4(){
    std::aligned_storage_t<128,alignof(int)> a;
    new (&a) int(10);
    std::cout<<*reinterpret_cast<int*>(&a)<<std::endl;//10
    std::cout<<sizeof(a)<<std::endl;//128，内存块大小
    //std::cout<<alignof(a)<<std::endl;//4，内存块对齐字节长度
}
//std::aligned_union，按照指定的类型列表，获取对应大小的内存块，类型的最大对齐字节长度即为内存块的对齐字节长度
void test5(){
    std::aligned_union_t<64,int,double> a;
    std::cout<<sizeof(a)<<std::endl;//64，内存块大小
    //std::cout<<alignof(a)<<std::endl;//8，内存块对齐字节长度
}
//std::align，在指定的缓冲区中，按照指定的对齐字节长度，获取对齐后的地址
void test6(){
    constexpr int align_size=32;
    constexpr int alloc_size=1024;
    constexpr int buf_size=align_size+alloc_size;
    char src[buf_size]{0};
    char dst[buf_size]{0};
    void* src_ori_ptr=src;
    void* dst_ori_ptr=dst;
    size_t dst_size=sizeof(dst);
    size_t src_size=sizeof(src);
    char* dst_ptr=static_cast<char*>(std::align(align_size,alloc_size,dst_ori_ptr,dst_size));
    char* src_ptr=static_cast<char*>(std::align(align_size,alloc_size,src_ori_ptr,src_size));
    std::cout<<std::hex<<reinterpret_cast<uintptr_t>(dst)<<std::endl;
    std::cout<<std::hex<<reinterpret_cast<uintptr_t>(dst_ptr)<<std::endl;
    std::cout<<std::hex<<reinterpret_cast<uintptr_t>(src)<<std::endl;
    std::cout<<std::hex<<reinterpret_cast<uintptr_t>(src_ptr)<<std::endl;
}
//C++17 new自动根据对齐字节长度，对齐内存块
struct alignas(32) T{
    char c;
    int i;
    double d;
};
void test7(){
    T* p=new T;
    std::cout<<std::hex<<reinterpret_cast<uintptr_t>(p)<<std::endl;
    delete p;    
}
int main(){
    //test1();
    //test2();
    //test3();
    //test4();
    //test5();
    //test6();
    test7();
    return 0;
}