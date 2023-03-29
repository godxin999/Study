/*************************************************************************
	> File Name: tcp_client_win.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/29 13:42:37
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
	int strlen=0;
	int idx=0,readlen=0;
	if(argc!=3){
		printf("Usage : %s <IP> <port>\n",argv[0]);
		exit(1);
	}

	if(WSAStartup(MAKEWORD(2,2),&wsadata)!=0){
		ErrorHandling("WSAStartup() error");
	}

	hSocket=socket(PF_INET,SOCK_STREAM,0);
	if(hSocket==INVALID_SOCKET){
		ErrorHandling("socket() error");
	}

	memset(&servAddr,0,sizeof(servAddr));
	servAddr.sin_family=AF_INET;
	servAddr.sin_addr.s_addr=inet_addr(argv[1]);
	servAddr.sin_port=htons(atoi(argv[2]));

	if(connect(hSocket,(SOCKADDR*)&servAddr,sizeof(servAddr))==SOCKET_ERROR){
		ErrorHandling("connect() error");
	}

	while(readlen=recv(hSocket,&message[idx++],1,0)){
		if(readlen==-1){
			ErrorHandling("read() error");
		}
		strlen+=readlen;
	}
	
	printf("Message from server: %s \n",message);
	printf("Function read call count: %d \n",strlen);
	closesocket(hSocket);
	WSACleanup();
	return 0;
}