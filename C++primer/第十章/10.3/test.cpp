/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/20 17:15:37
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;
bool isShorter(const string &s1,const string &s2){
	return s1.size()<s2.size();
}
void test1(){
	vector<string> words={"apple","banana","peach","pear","berry","watermelon","orange","lemon","mango"};
	sort(words.begin(),words.end());//按字典排序
	for(auto s:words){
		cout<<s<<endl;
	}
	stable_sort(words.begin(),words.end(),isShorter);//稳定排序以长短为谓词的同时保证相同长度的单词以字典序排列(相对位置不变)
	for(const auto &s:words){
		cout<<s<<endl;
	}
}

int main(){
	test1();





}

