/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/26 9:58:59
 ************************************************************************/
#include "TextQuery.h"
using namespace std;

void runQueries(ifstream& infile) {
	TextQuery tq(infile);
	while (true) {
		cout << "enter word to look for, or q to quit: "<<endl;
		string s;
		if (!(cin >> s) || s == "q") {
			break;
		}
		print(cout, tq.query(s)) << endl;
	}

}

int main(){
	ifstream fin("a.txt");
	runQueries(fin);
}

