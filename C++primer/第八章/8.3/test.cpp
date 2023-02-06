/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/2/6 20:58:29
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <vector>
using namespace std;

struct PersonInfo{
	string name;
	vector<string> phones;
};
vector<PersonInfo> people;
void read(){
	string line,word;
	while(getline(cin,line)){//按行读入
		PersonInfo info;
		istringstream record(line);//把该行绑定到istringstream对象
		record>>info.name;//从istringstream对象读入名字
		while(record>>word){//从istringstream对象读入号码
			info.phones.push_back(word);//存入号码数组
		}
		people.push_back(info);//存入个人信息数组
	}
}

void print(){
	for(const auto& entry:people){//范围for语句
		ostringstream formatted,badnums;
		for(const auto& nums:entry.phones){//对号码进行循环
			if(nums[0]=='5'){//如果号码首位是5就不是好好号码
				badnums<<" "<<nums;//输出到ostringstream对象中，实际上转换为字符串操作，向其后添加" "和nums
			}
			else{
				formatted<<" "<<nums;
			}
		}
		if(badnums.str().empty()){//如果ostringstream中所保存的string为空
			cout<<entry.name<<formatted.str()<<endl;//输出名字和正确的号码(formatted中保存的字符串)
		}
		else{
			cerr<<"input error: "<<entry.name<<" invalid numbers"<<badnums.str()<<endl;
		}
	}
}

int main(){
	read();
	print();

}

