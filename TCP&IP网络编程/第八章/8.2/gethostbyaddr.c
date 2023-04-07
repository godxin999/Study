/*************************************************************************
	> File Name: gethostbyaddr.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/7 10:41:05
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>

void error_handling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

int main(int argc,char **argv){
	int i;
	struct hostent *host;
	struct sockaddr_in addr;
	if(argc!=2){
		printf("Usage : %s <IP>\n",argv[0]);
		exit(1);
	}

	memset(&addr,0,sizeof(addr));
	addr.sin_addr.s_addr=inet_addr(argv[1]);//将main函数传递的IP地址字符串传递给inet_addr函数并填充进in_addr结构体中
	host=gethostbyaddr((char*)&addr.sin_addr,4,AF_INET);//因为需要传递IPv4地址外的其他信息，所以应该把in_addr结构体指针声明为char*类型
	if(!host){
		error_handling("gethost... error");
	}
	printf("Official name: %s \n",host->h_name);
	for(i=0;host->h_aliases[i];++i){
		printf("Aliases %d: %s \n",i+1,host->h_aliases[i]);
	}
	printf("Address type: %s \n",(host->h_addrtype==AF_INET)?"AF_INET":"AF_INET6");
	for(i=0;host->h_addr_list[i];++i){
		printf("IP addr %d: %s \n",i+1,inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
	}
	return 0;
}