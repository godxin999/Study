/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/17 20:42:47
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <sstream>
using namespace std;
//控制布尔值的格式
void test1(){
	cout<<"default bool values: "<<true<<" "<<false<<endl;//default bool values: 1 0
	cout<<"alpha bool values: "<<boolalpha<<true<<" "<<false<<endl;//alpha bool values: true false
	cout<<noboolalpha<<true<<" "<<false<<endl;//1 0，使用noboolalpha恢复为默认格式
}
//指定整型的进制
void test2(){
	cout<<"default: "<<20<<" "<<1024<<endl;//default: 20 1024
	cout<<"octal: "<<oct<<20<<" "<<1024<<endl;//octal: 24 2000
	cout<<"hex: "<<hex<<20<<" "<<1024<<endl;//hex: 14 400
	cout<<"decimal: "<<dec<<20<<" "<<1024<<endl;//decimal: 20 1024，这里将进制重新转换为10进制
}
//使用showbase在输出结果中显式进制
void test3(){
	cout<<showbase;
	cout<<"default: "<<20<<" "<<1024<<endl;//default: 20 1024
	cout<<"octal: "<<oct<<20<<" "<<1024<<endl;//octal: 024 02000
	cout<<"hex: "<<hex<<20<<" "<<1024<<endl;//hex: 0x14 0x400
	cout<<"decimal: "<<dec<<20<<" "<<1024<<endl;//decimal: 20 1024
	cout<<noshowbase;//恢复默认状态
	//使用upperbase在16进制中打印大写X
	cout<<uppercase<<showbase<<"hex: "<<hex<<20<<" "<<1024<<nouppercase<<noshowbase<<dec<<endl;//hex: 0X14 0X400
}
//控制浮点数精度
void test4(){
	//cout.precision()返回当前精度值，浮点数的精度默认为6位
	cout<<"Precision: "<<cout.precision()
		<<", Value: "<<sqrt(2.0)<<endl;//Precision: 6, Value: 1.41421
	cout.precision(12);//设置打印精度为12位
	cout<<"Precision: "<<cout.precision()
		<<", Value: "<<sqrt(2.0)<<endl;//Precision: 12, Value: 1.41421356237
	cout<<setprecision(3);//使用setprecision操纵符设置精度
	cout<<"Precision: "<<cout.precision()
		<<", Value: "<<sqrt(2.0)<<endl;//Precision: 3, Value: 1.41
}
//控制浮点数格式
void test5(){
	cout<<"default format: "<<100*sqrt(2.0)<<endl;//default format: 141.421
	//scientific操纵符表示使用科学计数法
	cout<<"scientific: "<<scientific<<100* sqrt(2.0)<<endl;//scientific: 1.414214e+02
	//fixed操纵符表示使用定点十进制
	cout<<"fixed decimal: "<<fixed<<100*sqrt(2.0)<<endl;//fixed decimal: 141.421356
	//hexfloat操纵符强制浮点数使用16进制格式
	cout<<"hexadecimal: "<<hexfloat<<100*sqrt(2.0)<<endl;//hexadecimal: 0x1.1ad7bc01366b8p+7
	//defaultfloat操纵符使流恢复默认状态
	cout<<"use default: "<<defaultfloat<<100*sqrt(2.0)<<endl;//use default: 141.421
}
//打印小数点
void test6(){
	cout<<10.0<<endl;//10
	cout<<showpoint<<10.0<<noshowpoint<<endl;//10.0000
}
//输出补白
void test7(){
	int i=-16;
	double d=3.14159;
	//setw表示下一个数字或者字符串使用输出中最少12个位置
	cout<<"i: "<<setw(12)<<i<<"next col"<<endl;
	cout<<"d: "<<setw(12)<<d<<"next col"<<endl;
	cout<<left;//左对齐
	cout<<"i: "<<setw(12)<<i<<"next col"<<endl;
	cout<<"d: "<<setw(12)<<d<<"next col"<<endl;
	cout<<right;//右对齐
	cout<<"i: "<<setw(12)<<i<<"next col"<<endl;
	cout<<"d: "<<setw(12)<<d<<"next col"<<endl;
	cout<<internal;//补白将会在中间
	cout<<"i: "<<setw(12)<<i<<"next col"<<endl;
	cout<<"d: "<<setw(12)<<d<<"next col"<<endl;
	cout<<setfill('#');//设置补白字符为'#'
	cout<<"i: "<<setw(12)<<i<<"next col"<<endl;
	cout<<"d: "<<setw(12)<<d<<"next col"<<endl;
	cout<<setfill(' ');
/*
i:          -16next col
d:      3.14159next col
i: -16         next col
d: 3.14159     next col
i:          -16next col
d:      3.14159next col
i: -         16next col
d:      3.14159next col
i: -#########16next col
d: #####3.14159next col
*/
}
//读取空格
void test8(){
	cin>>noskipws;//设置cin读取空白符
	char ch;
	while(cin>>ch){
		cout<<ch;
	}
	cin>>skipws;//恢复默认状态，丢弃空白符
}
//读写同一个文件
void test9(){
	fstream inOut("a.txt",fstream::ate|fstream::in|fstream::out);
	if(!inOut){
		cerr<<"Unable to open file!"<<endl;
		return ;
	}
	//inOut以ate模式打开，一开始就定位到其文件尾
	auto end_mark=inOut.tellg();//记录文件尾
	inOut.seekg(0,fstream::beg);//重定位到文件开始处
	size_t cnt=0;
	string line;
	while(inOut&&inOut.tellg()!=end_mark&&getline(inOut,line)){
		cnt+=line.size()+1;//+1用于统计换行符
		auto mark=inOut.tellg();//记录读取的位置
		inOut.seekp(0,fstream::end);//定义到尾部
		inOut<<cnt;//输出长度
		if(mark!=end_mark){
			inOut<<" ";
		}
		inOut.seekg(mark);//恢复读取位置
	}
	inOut.seekp(0,fstream::end);//定位到文件尾
	inOut<<"\n";
	return ;
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
	test9();
}