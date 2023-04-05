/*************************************************************************
	> File Name: uecho_server_win.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/5 12:14:28
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 30
void ErrorHandling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

int main(int argc,char **argv){
	WSADATA wsadata;
	SOCKET servSock;
	char message[BUF_SIZE];
	int strLen;
	int clntAdrSz;

	SOCKADDR_IN servAdr,clntAdr;
	if(argc!=2){
		printf("Usage %s <port>\n",argv[0]);
		exit(1);
	}
	if(WSAStartup(MAKEWORD(2,2),&wsadata)!=0){
		ErrorHandling("WSAStartup() error");
	}

	servSock=socket(PF_INET,SOCK_DGRAM,0);
	if(servSock==INVALID_SOCKET){
		ErrorHandling("socket() error");
	}

	memset(&servAdr,0,sizeof(servAdr));
	servAdr.sin_family=AF_INET;
	servAdr.sin_addr.s_addr=htonl(INADDR_ANY);
	servAdr.sin_port=htons(atoi(argv[1]));

	if(bind(servSock,(SOCKADDR*)&servAdr,sizeof(servAdr))==SOCKET_ERROR){
		ErrorHandling("bind() error");
	}

	while(1){
		clntAdrSz=sizeof(clntAdr);
		strLen=recvfrom(servSock,message,BUF_SIZE,0,(SOCKADDR*)&clntAdr,&clntAdrSz);
		sendto(servSock,message,strLen,0,(SOCKADDR*)&clntAdr,clntAdrSz);
	}
	closesocket(servSock);
	WSACleanup();
	return 0;
}