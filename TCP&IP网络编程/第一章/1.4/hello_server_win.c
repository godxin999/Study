/*************************************************************************
	> File Name: hello_server_win.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/27 17:31:17
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
//编译需要链接 ws2_32.lib库
void ErrorHandling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

int main(int argc,char **argv){
	WSADATA wsaData;
	SOCKET hServSock,hClntSock;
	SOCKADDR_IN servAddr,clntAddr;

	int szClntAddr;
	char message[]="Hello World!";
	if(argc!=2){
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}

	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0){//初始化套接字库
		ErrorHandling("WSAStartup() error");
	}
	
	hServSock=socket(PF_INET,SOCK_STREAM,0);//创建套接字
	if(hServSock==INVALID_SOCKET){
		ErrorHandling("socket() error");
	}

	memset(&servAddr,0,sizeof(servAddr));
	servAddr.sin_family=AF_INET;
	servAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servAddr.sin_port=htons(atoi(argv[1]));

	if(bind(hServSock,(SOCKADDR*)&servAddr,sizeof(servAddr))==SOCKET_ERROR){//分配IP地址和端口号
		ErrorHandling("bind() error");
	}

	if(listen(hServSock,5)==SOCKET_ERROR){//使用listen函数是之前创建的套接字成为服务器端套接字
		ErrorHandling("listen() error");
	}

	szClntAddr=sizeof(clntAddr);
	hClntSock=accept(hServSock,(SOCKADDR*)&clntAddr,&szClntAddr);//使用accept函数受理客户端连接请求
	if(hClntSock==INVALID_SOCKET){
		ErrorHandling("accept() error");
	}

	send(hClntSock,message,sizeof(message),0);//使用send函数向之前连接的客户端传输数据
	closesocket(hClntSock);
	closesocket(hServSock);
	WSACleanup();//注销套接字库
	return 0;
}