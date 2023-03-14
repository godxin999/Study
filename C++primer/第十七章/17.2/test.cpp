/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/14 16:42:48
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <bitset>
using namespace std;
//使用unsigned值初始化bitset
void test1(){
	bitset<13> bitvec1(0xbeef);//bitvec1比初始值小，初始值中的高位被丢弃
	bitset<20> bitvec2(0xbeef);//bitvec2比初始值大，高位被置零
	bitset<128> bitvec3(~0ULL);//0~63位为1，63~127位为0
	cout<<bitvec1<<endl;//1111011101111
	cout<<bitvec2<<endl;//00001011111011101111
	cout<<bitvec3<<endl;
}
//使用string初始化bitset
void test2(){
	bitset<16> bitvec1("1100");
	string str="11111111000000011001101";
	bitset<16> bitvec2(str,5,4);//从str[5]开始的四个二进制位
	bitset<16> bitvec3(str,str.size()-4);//使用倒数4个字符进行初始化，第二个参数为开始位置，即使用倒数第四位到最后一位进行初始化
	cout<<bitvec1<<endl;//0000000000001100
	cout<<bitvec2<<endl;//0000000000001110
	cout<<bitvec3<<endl;//0000000000001101
}
//bitset操作
void test3(){
	bitset<16> bitvec(1U);
	bool is_set=bitvec.any();//是否有置位的二进制位
	bool is_not_set=bitvec.none();//是否均未置位
	bool all_set=bitvec.all();//是否全部置位
	cout<<is_set<<" "<<is_not_set<<" "<<all_set<<endl;//1 0 0
	size_t onBits=bitvec.count();//置位的数量
	size_t sz=bitvec.size();//返回位数
	cout<<onBits<<" "<<sz<<endl;//1 16
	bitvec.flip();//改变所有位的状态
	cout<<bitvec<<endl;//1111111111111110
	bitvec.reset();//全部复位
	cout<<bitvec<<endl;//0000000000000000
	bitvec.set();//全部置位
	cout<<bitvec<<endl;//1111111111111111
	bitvec.flip(0);//翻转第一位
	cout<<bitvec<<endl;//1111111111111110
	bitvec.set(bitvec.size()-1);//置位最后一位
	cout<<bitvec<<endl;//1111111111111110
	bitvec.set(5,0);//复位第五位
	cout<<bitvec<<endl;//1111111111011110
	bitvec.reset(7);//复位第七位
	cout<<bitvec<<endl;//1111111101011110
	cout<<bitvec.test(0)<<endl;//判断某位是否置位，这里第一位没有置位，所以返回0
}
//利用下标进行操作
void test4(){
	bitset<16> bitvec(~1U);
	cout<<bitvec<<endl;//1111111111111110
	bitvec[3]=0;
	cout<<bitvec<<endl;//1111111111110110
	bitvec[15]=bitvec[0];
	cout<<bitvec<<endl;//0111111111110110
	bitvec[0].flip();
	cout<<bitvec<<endl;//0111111111110111
	//~bitvec[0];//不合法
	//cout<<bitvec<<endl;//0111111111110111
	bool b=bitvec[0];
}
//提取bitset的值
void test5(){
	bitset<16> bitvec(~1U);
	unsigned long ulong=bitvec.to_ulong();
	cout<<"ulong = "<<ulong<<endl;//65534
}
//bitset with IO
void test6(){
	bitset<16> bits;
	cin>>bits;//从cin读取16个0或1
	cout<<"bits "<<bits<<endl;
}

int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	test6();
}

