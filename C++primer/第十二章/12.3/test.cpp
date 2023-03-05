/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/26 9:58:59
 ************************************************************************/
#include "Query.h"
using namespace std;

void runQueries(ifstream& infile) {
	TextQuery tq(infile);/*
	while (true) {
		cout << "enter word to look for, or q to quit: "<<endl;
		string s;
		if (!(cin >> s) || s == "q") {
			break;
		}
		print(cout, tq.query(s)) << endl;
	}
	*/
	cout << "test &" << endl;
	string s1, s2, s3;
	cin >> s1 >> s2;
	Query q = Query (s1) & Query (s2);;
	const auto result1 = q.eval(tq);
	print(cout, result1);
	cout << "test |" << endl;
	cin >> s1 >> s2;
	q = Query(s1) | Query(s2);
	const auto result2 = q.eval(tq);
	print(cout, result2);
	cout << "test ~" << endl;
	cin >> s1;
	q = ~Query(s1);
	const auto result3 = q.eval(tq);
	print(cout, result3);
}

int main(){
	ifstream fin("a.txt");
	runQueries(fin);
}

