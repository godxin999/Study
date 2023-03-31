/*************************************************************************
	> File Name: conv_addr_win.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/31 17:50:15
 ************************************************************************/

#undef UNICODE//取消定义的宏，防止函数参数转换为unicode形式从而导致错误
#undef _UNICODE
#include <stdio.h>
#include <WinSock2.h>

int main(int argc,char **argv){
	char *strAddr="203.211.218.102:9190";

	char strAddrBuf[50];
	SOCKADDR_IN servAddr;
	int size;
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2,2),&wsadata);

	size=sizeof(servAddr);
	WSAStringToAddress(strAddr,AF_INET,NULL,(SOCKADDR*)&servAddr,&size);//将地址信息字符串填入结构体变量

	size=sizeof(strAddrBuf);
	WSAAddressToString((SOCKADDR*)&servAddr,sizeof(servAddr),NULL,strAddrBuf,&size);//将结构体中的地址信息转换成字符串形式

	printf("Second conv result: %s \n",strAddrBuf);
	WSACleanup();


	return 0;
}
