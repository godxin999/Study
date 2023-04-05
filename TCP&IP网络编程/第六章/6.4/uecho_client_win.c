/*************************************************************************
	> File Name: uecho_client_win.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/5 12:24:32
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
	SOCKET sock;
	char message[BUF_SIZE];
	int strLen;

	SOCKADDR_IN servAdr;
	if(argc!=3){
		printf("Usage : %s <IP> <port>\n",argv[0]);
		exit(1);
	}
	if(WSAStartup(MAKEWORD(2,2),&wsadata)!=0){
		ErrorHandling("WSAStartup() error");
	}

	sock=socket(PF_INET,SOCK_DGRAM,0);
	if(sock==INVALID_SOCKET){
		ErrorHandling("socket() error");
	}

	memset(&servAdr,0,sizeof(servAdr));
	servAdr.sin_family=AF_INET;
	servAdr.sin_addr.s_addr=inet_addr(argv[1]);
	servAdr.sin_port=htons(atoi(argv[2]));

	connect(sock,(SOCKADDR*)&servAdr,sizeof(servAdr));

	while(1){
		fputs("Insert message(Q to quit): ",stdout);
		fgets(message,sizeof(message),stdin);
		if(!strcmp(message,"q\n")||!strcmp(message,"Q\n")){
			break;
		}

		send(sock,message,strlen(message),0);
		strLen=recv(sock,message,BUF_SIZE-1,0);
		message[strLen]=0;
		printf("Message from server: %s",message);
	}
	closesocket(sock);
	WSACleanup();
	return 0;
}