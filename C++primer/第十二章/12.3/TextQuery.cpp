#include "TextQuery.h"
using namespace std;

TextQuery::TextQuery(ifstream& is) :file(new vector<string>) {
	string text;
	while (getline(is, text)) {//输入文件的每一行
		file->push_back(text);//保存此行文本
		auto n = file->size() - 1;//当前行号
		istringstream line(text);//将每行分解为单词
		string word;
		while (line >> word) {//对行中每个单词
			auto& lines = wm[word];//lines为一个shared_ptr，引用类型可以对其进行修改，如果单词未出现过，map的下标运算符会默认将其加入
			if (!lines) {//如果第一次遇到这个单词，该指针为空
				lines.reset(new set<line_no>);//为这个单词分配新的set，智能指针指向这个set
			}
			lines->insert(n);//在对应set中插入行号
		}
	}
}

QueryResult TextQuery::query(const string& sought)const {
	static shared_ptr<set<line_no>> nodata(new set<line_no>);//定义一个静态对象，在没有查询到单词时，返回该set的指针
	auto loc = wm.find(sought);//使用find进行查询，以避免下标运算符将单词加入到wm中
	if (loc == wm.end()) {
		return QueryResult(sought, nodata, file);//未找到
	}
	else {
		return QueryResult(sought, loc->second, file);
	}
}