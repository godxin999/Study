/*************************************************************************
	> File Name: main.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/22 9:13:59
 ************************************************************************/

#include "Panda.h"
using namespace std;
/*
void print(const Bear&) {
	cout << "print(const Bear&)" << endl;
}
void highlight(const Endangered&) {
	cout << "highlight(const Endangered&)" << endl;
}
ostream& operator<<(ostream& os, const ZooAnimal&) {
	return os << "ZooAnimal output operator" << endl;
}

int main(){
	Panda yingyang("yingyang");
	print(yingyang);
	highlight(yingyang);
	cout << yingyang << endl;
	//Panda lingling = yingyang;
}
*/
void dance(const Bear&) {
	cout << "dance(const Bear&)" << endl;
}
void rummage(const Raccoon&) {
	cout << "rummage(const Raccoon&)" << endl;
}
ostream& operator<<(ostream& os, const ZooAnimal&) {
	return os << "ZooAnimal output operator" << endl;
}

int main() {
	Panda yingyang;
	//Panda yingyang("panda1");
	//dance(yingyang);
	//rummage(yingyang);
	//cout << yingyang << endl;
}
