/*************************************************************************
	> File Name: inet_adrconv_win.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/31 17:28:56
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <WinSock2.h>

void ErrorHandling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

int main(int argc,char **argv){
	WSADATA wsadata;
	if(WSAStartup(MAKEWORD(2,2),&wsadata)!=0){
		ErrorHandling("WSAStartup() error");
	}
	//inet_addr函数
	{
		char *addr="127.212.124.78";
		unsigned long conv_addr=inet_addr(addr);//将字符串ip地址转换为32位无符号整数，并将其转换为网络字节序
		if(conv_addr==INADDR_NONE){
			ErrorHandling("Error occured \n");
		}
		else{
			printf("Network ordered integer addr: %#lx \n",conv_addr);
		}
	}
	//inet_ntoa函数
	{
		struct sockaddr_in addr;
		char *strPtr;
		char strArr[20];

		addr.sin_addr.s_addr=htonl(0x1020304);
		strPtr=inet_ntoa(addr.sin_addr);//将ip地址信息转换为字符串形式的ip地址
		strcpy(strArr,strPtr);
		printf("Dotted-Decimal notation %s \n",strArr);
	}
	WSACleanup();
	return 0;
}