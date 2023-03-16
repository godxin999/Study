/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/16 19:54:44
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <regex>
using namespace std;
//使用正则表达式
void test1(){
	string pattern("[^c]ei");//查找不在c之后的字符串ei
	pattern="[[:alpha:]]*"+pattern+"[[:alpha:]]*";//[[:alpha:]]*表示可以匹配0个或多个字母
	regex r(pattern);//构造一个用于查找模式的regex
	smatch results;//定义一个对象保存搜索结果
	string test_str="receipt freind theif receive";
	if(regex_search(test_str,results,r)){//如果有匹配子串就返回true
		cout<<results.str()<<endl;//打印匹配的单词，因为regex_search在找到一个匹配子串后就会停止，所以输出为freind
	}
}
//使用标志来影响regex的操作，icase表示不区分大小写
void test2(){
	//匹配一个或多个字母或数字字符后接一个'.'再接"cpp"或"cxx"或"cc"的字符串
	regex r("[[:alnum:]]+\\.(cpp|cxx|cc)$",regex::icase);//两个反斜杠转义得到一个反斜杠，反斜杠+'.'去除正则表达式中'.'的特殊含义，一般的'.'可以匹配任意字符，这里只想要匹配'.'
	smatch results;
	string filename;
	while(cin>>filename){
		if(regex_search(filename,results,r)){
			cout<<results.str()<<endl;
		}
	}
}
//正则表达式发生错误时会抛出regex_error
void test3(){
	try{
		regex r("[[:alnum:]+\\.(cpp|cxx|cc)$",regex::icase);
	}
	catch (regex_error e){
		cout<<e.what()<<"\ncode: "<<e.code()<<endl;
	}
/*
regex_error(error_brack): The expression contained mismatched [ and ].
code: 4
*/
}
//使用的RE库类型要与输入类型匹配
void test4(){
	regex r("[[:alnum:]]+\\.(cpp|cxx|cc)$",regex::icase);
	//smatch results;//只能匹配string类型
	cmatch results;//匹配字符数组
	if(regex_search("myfile.cc",results,r)){
		cout<<results.str()<<endl;
	}
}
//使用sregex_iterator获取全部匹配字符串
void test5(){
	string pattern("[^c]ei");
	pattern="[[:alpha:]]*"+pattern+"[[:alpha:]]*";
	regex r(pattern,regex::icase);
	smatch results;
	string test_str="receipt freind theif receive";
	//反复调用regex_search来寻找文件中匹配的串
	for(sregex_iterator it(test_str.begin(),test_str.end(),r),end_it;it!=end_it;++it){//it为指向匹配位置的迭代器，end_it为尾后迭代器，每次递增it都会跳到下一个匹配的位置
		cout<<it->str()<<endl;
	}
}
//获取匹配部分的上下文
void test6(){
	string pattern("[^c]ei");
	pattern="[[:alpha:]]*"+pattern+"[[:alpha:]]*";
	regex r(pattern,regex::icase);
	string test_str="receipt freind theif receive";
	for(sregex_iterator it(test_str.begin(),test_str.end(),r),end_it;it!=end_it;++it){
		auto pos=it->prefix().length();//获取前缀的大小
		pos=pos>40?pos-40:0;//最多只要40个字符
		cout<<it->prefix().str().substr(pos)//使用substr()来获取指定位置到前缀末尾部分的字符串
			<<"\n\t\t>>> "<<it->str()<<" <<<\n"//匹配得到的单词
			<<it->suffix().str().substr(0,40)//使用substr()获取部分后缀
			<<endl;
	}
}
//使用子表达式
void test7(){
	regex r("([[:alnum:]]+)\\.(cpp|cxx|cc)$",regex::icase);//括号括起的地方为一个子表达式，r中含有两个子表达式
	smatch results;
	string filename;
	while(cin>>filename){
		if(regex_search(filename,results,r)){
			cout<<results.str(0)<<endl;//打印整个模式对应的匹配
			cout<<results.str(1)<<endl;//打印第一个子表达式
			cout<<results.str(2)<<endl;//打印第二个子表达式
		}
	}
}
//使用子匹配操作匹配合法美国电话号
bool valid(const smatch& m){
	if(m[1].matched){
		return m[3].matched&&(m[4].matched==0||m[4].str()==" ");//匹配两个括号且区号后没有分隔符或者有一个空格
	}
	else return !m[3].matched&&m[4].str()==m[6].str();//两个分隔符要相同
}
void test8(){
	//\{d}表示单个数字，\{d}{n}表示n个数字的序列，反斜杠需要转义
	//[]中的字符集合表示匹配这些字符中的任意一个，点在括号中没有特殊含义
	//后接'?'的组件是可选的
	string phone="(\\()?(\\d{3})(\\))?([-. ])?(\\d{3})([-. ])?(\\d{4})";//含有七个子表达式，第一个是区号部分可选的左括号，第二个是三位数字，表示区号，第三个是区号部分可选的右括号，第四个表示区号部分可选的分隔符，第五个表示号码的下三位数字，第六个表示可选的分隔符，第七个表示号码的最后四位数字
	regex r(phone);
	smatch m;
	string s;
	while(getline(cin,s)){
		for(sregex_iterator it(s.begin(),s.end(),r),end_it;it!=end_it;++it){
			if(valid(*it)){
				cout<<"valid: "<<it->str()<<endl;
			}
			else{
				cout<<"not valid: "<<it->str()<<endl;
			}
		}
	}
}
//regex_replace
void test9(){
	string phone="(\\()?(\\d{3})(\\))?([-. ])?(\\d{3})([-. ])?(\\d{4})";
	regex r(phone);
	string fmt="$2.$5.$7";//$后跟子表达式索引号来表示特定的子表达式，这里把号码格式改为xxx.xxx.xxxx
	string number="(908) 555-1800";
	cout<<regex_replace(number,r,fmt)<<endl;//908.555.1800
}
//使用格式标志
void test10(){
	using namespace std::regex_constants;//匹配和格式化标志均定义在这个命名空间中
	string phone="(\\()?(\\d{3})(\\))?([-. ])?(\\d{3})([-. ])?(\\d{4})";
	regex r(phone);
	string fmt="$2.$5.$7 ";
	string number="Tom: (908) 555-1800";
	cout<<regex_replace(number,r,fmt,format_no_copy)<<endl;//908.555.1800 ，format_no_copy表示只拷贝其替换的文本
}

int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	//test6();
	//test7();
	//test8();
	//test9();
	test10();
}

