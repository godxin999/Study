/*************************************************************************
	> File Name: gethostbyname_win.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/7 15:21:36
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

void ErrorHandling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

int main(int argc,char **argv){
	WSADATA wsadata;
	int i;
	struct hostent *host;
	if(argc!=2){
		printf("Usage : %s <addr> \n",argv[0]);
		exit(1);
	}
	if(WSAStartup(MAKEWORD(2,2),&wsadata)!=0){
		ErrorHandling("WSAStartup() error");
	}

	host=gethostbyname(argv[1]);
	if(!host){
		ErrorHandling("gethost... error");
	}

	printf("Official name: %s \n",host->h_name);
	for(i=0;host->h_aliases[i];++i){
		printf("Aliases %d: %s \n",i+1,host->h_aliases[i]);
	}
	printf("Address type: %s \n",(host->h_addrtype==AF_INET)?"AF_INET":"AF_INET6");
	for(i=0;host->h_addr_list[i];++i){
		printf("IP addr %d: %s \n",i+1,inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
	}
	WSACleanup();
	return 0;
}