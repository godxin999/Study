/*************************************************************************
	> File Name: testBlob.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/7 16:20:29
 ************************************************************************/

#include "BlobPtr.h"
#include <list>
using namespace std;

int main(){
	/*
	Blob<int> ia;
	Blob<int> ia2 = { 1,2,3,4,5 };
	Blob<double> price;
	Blob<string> names;
	Blob<int> squares = { 0,1,2,3,4,5,6,7,8 };
	for (size_t i = 0; i != squares.size(); ++i) {
		squares[i] = i * i;
	}
	BlobPtr<int> p(ia2);
	cout << *++p << endl;
	cout << (squares == ia2) << endl;
	*/
	int ia[] = { 1,2,3,4,5,6,7,8,9 };
	vector<int> vi = { 1,2,3,4,5,6,7,8,9 };
	list<const char*> w = { "now","is","the","time" };
	Blob<int> a1(begin(ia), end(ia));//实例化为Blob<int>::Blob(int*,int*)
	Blob<int> a2(vi.begin(), vi.end());//使用vector<int>::iterator替换It来实例化构造函数
	Blob<string> a3(w.begin(), w.end());



}

