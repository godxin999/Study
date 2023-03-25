/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/24 23:04:06
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <set>
#include <map>
#include <memory>
using namespace std;
//嵌套类
class TextQuery{
public:
	class QueryResult;//嵌套类声明，嵌套类不是外层类的成员
	typedef vector<string>::size_type line_no;
	QueryResult query(const string&)const;
private:
	shared_ptr<vector<string>> file;
	map<string,shared_ptr<set<line_no>>> wm;
};
//在外层类外定义嵌套类，
class TextQuery::QueryResult{
public:
	QueryResult(string,shared_ptr<set<line_no>>,shared_ptr<vector<string>>);//嵌套类可以直接使用外层类的成员line_no
	void get_sought(){
		cout<<sought<<endl;
	}
private:
	string sought;
	shared_ptr<set<line_no>> lines;
	shared_ptr<vector<string>> file;

};
//在外层类外定义嵌套类的构造函数
inline
TextQuery::QueryResult::QueryResult(string s,shared_ptr<set<line_no>> p,shared_ptr<vector<string>> f):sought(s),lines(p),file(f){}

TextQuery::QueryResult
TextQuery::query(const string &sought)const{
	static shared_ptr<set<line_no>> nodata(new set<line_no>);
	return QueryResult("test",nodata,file);
}

int main(){
	TextQuery t;
	t.query("").get_sought();
}