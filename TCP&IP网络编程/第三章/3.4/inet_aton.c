/*************************************************************************
	> File Name: inet_aton.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/31 15:40:22
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

void error_handling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);	
}

int main(int argc,char **argv){
	char *addr="127.232.124.79";
	struct sockaddr_in addr_inet;

	if(!inet_aton(addr,&addr_inet.sin_addr)){//将ip地址字符串转换为网络字节序并填入sockaddr_in结构体中
		error_handling("Conversion error");
	}
	else{
		printf("Network ordered integer addr: %#x \n",addr_inet.sin_addr.s_addr);
	}
	return 0;
}