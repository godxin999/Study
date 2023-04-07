/*************************************************************************
	> File Name: gethostbyname.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/7 10:07:40
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
	if(argc!=2){
		printf("Usage : %s <addr>\n",argv[0]);
		exit(1);
	}

	host=gethostbyname(argv[1]);//以main函数传递的字符串作为参数传递给gethostbyname
	if(!host){
		error_handling("gethost... error");
	}

	printf("Official name: %s \n",host->h_name);//输出官方域名
	for(i=0;host->h_aliases[i];++i){
		printf("Aliases %d: %s \n",i+1,host->h_aliases[i]);//输出除官方域名外的域名
	}
	printf("Address type: %s \n",(host->h_addrtype==AF_INET)?"AF_INET":"AF_INET6");
	for(i=0;host->h_addr_list[i];++i){
		//h_addr_list指针数组中的元素实际指向的是in_addr结构体变量而不是字符串，所以需要强制转换为in_addr*类型，并传递给inet_ntoa函数得到字符串形式的IP地址
		printf("IP addr %d: %s \n",i+1,inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));//输出IP地址信息
	}
	return 0;
}