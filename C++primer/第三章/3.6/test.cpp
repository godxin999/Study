/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/1/9 13:31:55
 ************************************************************************/
//多维数组
#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
//对多维数组中数组的引用
void test1(){
    int ia[3][4]={
        {0,1,2,3},
        {4,5,6,7},
        {8,9,10,11}
    };
    int (&row)[4]=ia[1];//含有四个数的数组的引用，绑定到ia的第二行
    for(auto i:row){
        cout<<i<<" ";
    }
    cout<<endl;
    row[1]=666;//修改第二个元素
    for(auto i:row){
        cout<<i<<" ";
    }
    cout<<endl;
}
//range for遍历多维数组
void test2(){
    int ia[3][4]={
        {0,1,2,3},
        {4,5,6,7},
        {8,9,10,11}
    };
    for(auto &row:ia){
        for(auto &col:row){
            cout<<col<<" ";
        }
        cout<<endl;
    }
//range for遍历多维数组最内层可以不加引用，其他外层循环要声明成引用类型，防止数组被自动转为指针
    for(auto &row:ia){
        for(auto col:row){
            cout<<col<<" ";
        }
        cout<<endl;
    }
}
//指向多维数组中数组的指针
void test3(){
    int ia[3][4]={
        {0,1,2,3},
        {4,5,6,7},
        {8,9,10,11}
    };
    int (*p)[4]=ia;//指向含有四个数的数组的指针
    p=&ia[2];//指向ia第三行
    for(auto i:*p){
        cout<<i<<" ";
    }
    cout<<endl;
    *(*p+1)=666;//修改第二个元素
    for(auto i:*p){
        cout<<i<<" ";
    }
}
//使用指针遍历多维数组
void test4(){
    int ia[3][4]={
        {0,1,2,3},
        {4,5,6,7},
        {8,9,10,11}
    };
    for(auto p=ia;p!=ia+3;++p){
        for(auto q=*p;q!=*p+4;++q){
            cout<<*q<<" ";
        }
        cout<<endl;
    }

    for(auto p=begin(ia);p!=end(ia);++p){
        for(auto q=begin(*p);q!=end(*p);++q){
            cout<<*q<<" ";
        }
        cout<<endl;
    }
}
//类型别名简化
void test5(){
    int ia[3][4]={
        {0,1,2,3},
        {4,5,6,7},
        {8,9,10,11}
    };
    using int_array=int[4];//int_array是四个整数组成的数组
    //typedef int int_array[4];
    for(int_array *p=ia;p!=ia+3;++p){
        for(int *q=*p;q!=*p+4;++q){
            cout<<*q<<" ";
        }
        cout<<endl;
    }
}

int main(){
    //test1();
    //test2();
    //test3();
    //test4();
    test5();

}

