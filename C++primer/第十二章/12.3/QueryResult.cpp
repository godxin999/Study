#include "QueryResult.h"
using namespace std;

ostream& print(ostream& os, const QueryResult& qr) {
	os << qr.sought << " occurs " << qr.lines->size() << " ";
	auto f = [](auto i)->string {
		if (i == 1)return "time";
		else return "times";
	};
	os << f(qr.lines->size()) << endl;
	for (auto num : *qr.lines) {//当未找到单词时，set为空，该函数不执行
		os << "\t(line " << num + 1 << ") " << *(qr.file->begin() + num) << endl;
	}
	return os;
}
