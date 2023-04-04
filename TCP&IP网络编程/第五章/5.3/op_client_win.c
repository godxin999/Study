/*************************************************************************
	> File Name: op_client_win.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/4 19:31:36
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4

void ErrorHandling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

int main(int argc,char **argv){
	WSADATA wsadata;
	SOCKET hSocket;
	char opmsg[BUF_SIZE];
	int result,opndCnt,i;
	SOCKADDR_IN servAdr;
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

	memset(&servAdr,0,sizeof(servAdr));
	servAdr.sin_family=AF_INET;
	servAdr.sin_addr.s_addr=inet_addr(argv[1]);
	servAdr.sin_port=htons(atoi(argv[2]));

	if(connect(hSocket,(SOCKADDR*)&servAdr,sizeof(servAdr))==SOCKET_ERROR){
		ErrorHandling("connect() error");
	}
	else{
		puts("Connected.........");
	}

	fputs("Operand count: ",stdout);
	scanf("%d",&opndCnt);
	opmsg[0]=(char)opndCnt;//约定以1字节整数形式传递待运算整数的个数
	for(i=0;i<opndCnt;++i){
		printf("Operand %d: ",i+1);
		scanf("%d",(int*)&opmsg[i*OPSZ+1]);//约定以四字节形式传递运算数，转换为int*指针保存在char数组中
	}
	fgetc(stdin);//删除缓冲中的字符\n
	fputs("Operator: ",stdout);
	scanf("%c",&opmsg[opndCnt*OPSZ+1]);//输入运算符信息
	send(hSocket,opmsg,opndCnt*OPSZ+2,0);
	recv(hSocket,&result,RLT_SIZE,0);//结果约定为4字节整数，故一次read调用即可接收

	printf("Operation result: %d \n",result);
	closesocket(hSocket);
	WSACleanup();
	return 0;
}

