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
void test1(){
    int ia[3][4]={
        {0,1,2,3},
        {4,5,6,7},
        {8,9,10,11}
    };
    int (&row)[4]=ia[1];
    for(auto i:row){
        cout<<i<<" ";
    }
    cout<<endl;
    row[1]=666;
    for(auto i:row){
        cout<<i<<" ";
    }
    cout<<endl;
}

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

    for(auto &row:ia){
        for(auto col:row){
            cout<<col<<" ";
        }
        cout<<endl;
    }

}

void test3(){
    int ia[3][4]={
        {0,1,2,3},
        {4,5,6,7},
        {8,9,10,11}
    };
    int (*p)[4]=ia;
    p=&ia[2];
    for(auto i:*p){
        cout<<i<<" ";
    }
    cout<<endl;
    *(*p+1)=666;
    for(auto i:*p){
        cout<<i<<" ";
    }

}

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

void test5(){
    int ia[3][4]={
        {0,1,2,3},
        {4,5,6,7},
        {8,9,10,11}
    };
    using int_array=int[4];
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

