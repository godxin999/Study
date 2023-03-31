/*************************************************************************
	> File Name: endian_conv_win.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/31 17:12:41
 ************************************************************************/

#include <stdio.h>
#include <WinSock2.h>

void ErrorHandling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

int main(int argc,char **argv){
	WSADATA wsadata;
	unsigned short host_port=0x1234;
	unsigned short net_port;
	unsigned long host_addr=0x12345678;
	unsigned long net_addr;

	if(WSAStartup(MAKEWORD(2,2),&wsadata)!=0){
		ErrorHandling("WSAStartup() error");
	}

	net_port=htons(host_port);
	net_addr=htonl(host_addr);

	printf("Host ordered port: %#x \n",host_port);
	printf("Network ordered port: %#x \n",net_port);
	printf("Host ordered address: %#lx \n",host_addr);
	printf("Network ordered address: %#lx \n",net_addr);
	WSACleanup();
	return 0;
}