/*************************************************************************
	> File Name: .\hello_client_win.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/27 17:49:15
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

void ErrorHandling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

int main(int argc,char **argv){
	WSADATA wsadata;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30];
	int strlen;
	if(argc!=3){
		printf("Usage : %s <IP> <port>\n",argv[0]);
		exit(1);
	}

	if(WSAStartup(MAKEWORD(2,2),&wsadata)!=0){//初始化Winsock库
		ErrorHandling("WSAStartup() error");
	}

	hSocket=socket(PF_INET,SOCK_STREAM,0);//创建套接字
	if(hSocket==INVALID_SOCKET){
		ErrorHandling("socket() error");
	}

	memset(&servAddr,0,sizeof(servAddr));
	servAddr.sin_family=AF_INET;
	servAddr.sin_addr.s_addr=inet_addr(argv[1]);
	servAddr.sin_port=htons(atoi(argv[2]));

	if(connect(hSocket,(SOCKADDR*)&servAddr,sizeof(servAddr))==SOCKET_ERROR){//向服务器发送链接请求
		ErrorHandling("connect() error");
	}

	strlen=recv(hSocket,message,sizeof(message)-1,0);//使用recv函数接受服务器发来的数据
	if(strlen==-1){
		ErrorHandling("read() error");
	}
	printf("Message from server: %s \n",message);
	closesocket(hSocket);
	WSACleanup();//注销Winsock库
	return 0;
}