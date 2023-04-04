/*************************************************************************
	> File Name: op_server_win.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/4 19:31:43
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <WinSock2.h>

#define BUF_SIZE 1024
#define OPSZ 4
void ErrorHandling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

int claculate(int opnum,int opnds[],char op){
	int result=opnds[0],i;
	switch (op)
	{
	case '+':
		for(i=1;i<opnum;++i){
			result+=opnds[i];
		}
		break;
	case '-':
		for(i=1;i<opnum;++i){
			result-=opnds[i];
		}
		break;
	case '*':
		for(i=1;i<opnum;++i){
			result*=opnds[i];
		}
		break;
	}
	return result;
}

int main(int argc,char **argv){
	WSADATA wsadata;
	SOCKET hServSock,hClntSock;
	char opinfo[BUF_SIZE];
	int result,i,opndCnt;
	int recvCnt,recvLen;
	SOCKADDR_IN servAdr,clntAdr;
	int clntAdrSize;
	if(argc!=2){
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}

	if(WSAStartup(MAKEWORD(2,2),&wsadata)!=0){
		ErrorHandling("WSAStartup() error");
	}

	hServSock=socket(PF_INET,SOCK_STREAM,0);
	if(hServSock==INVALID_SOCKET){
		ErrorHandling("socket() error");
	}
	
	memset(&servAdr,0,sizeof(servAdr));
	servAdr.sin_family=AF_INET;
	servAdr.sin_addr.s_addr=htonl(INADDR_ANY);
	servAdr.sin_port=htons(atoi(argv[1]));

	if(bind(hServSock,(SOCKADDR*)&servAdr,sizeof(servAdr))==SOCKET_ERROR){
		ErrorHandling("bind() error");
	}
	if(listen(hServSock,5)==SOCKET_ERROR){
		ErrorHandling("listen() error");
	}
	clntAdrSize=sizeof(clntAdr);

	for(i=0;i<1;++i){
		opndCnt=0;
		hClntSock=accept(hServSock,(SOCKADDR*)&clntAdr,&clntAdrSize);
		recv(hClntSock,&opndCnt,1,0);//读取待运算数的个数

		recvLen=0;
		while((opndCnt*OPSZ+1)>recvLen){//读取带运算数
			recvCnt=recv(hClntSock,&opinfo[recvLen],BUF_SIZE-1,0);
			recvLen+=recvCnt;
		}
		result=claculate(opndCnt,(int*)opinfo,opinfo[recvLen-1]);//长度为recvLen，下标位置为recvLen-1
		send(hClntSock,(char*)&result,sizeof(result),0);//将整形结果以char类型发送回客户端
		closesocket(hClntSock);
	}
	closesocket(hServSock);
	WSACleanup();
	return 0;
}