/*************************************************************************
	> File Name: testBlob.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/7 16:20:29
 ************************************************************************/

#include "BlobPtr.h"
using namespace std;

int main(){
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


}

