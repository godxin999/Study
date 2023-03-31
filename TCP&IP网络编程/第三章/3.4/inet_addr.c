/*************************************************************************
	> File Name: inet_addr.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/31 11:05:47
 ************************************************************************/

#include <stdio.h>
#include <arpa/inet.h>

int main(int argc,char **argv){
	char *addr1="1.2.3.4";
	char *addr2="1.2.3.256";//一个字节能表示的最大整数为255，所以这是一个错误的ip地址
	//inet_addr函数将ip地址转换为32位无符号整形，并将其转换为网络字节序
	unsigned long conv_addr=inet_addr(addr1);
	if(conv_addr==INADDR_NONE){
		printf("Error occured \n");
	}
	else{
		printf("Network ordered integer addr: %#lx \n",conv_addr);
	}

	conv_addr=inet_addr(addr2);//调用出现异常，返回INADDR_NONE
	if(conv_addr==INADDR_NONE){
		printf("Error occured \n");
	}
	else{
		printf("Network ordered integer addr: %#lx \n\n",conv_addr);
	}
	return 0;
}