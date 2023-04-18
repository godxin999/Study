/*************************************************************************
	> File Name: sock_type.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/18 8:07:33
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

void error_handling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

int main(int argc,char **argv){
	int tcp_sock,udp_sock;
	int sock_type;
	socklen_t optlen;
	int state;

	optlen=sizeof(sock_type);
	tcp_sock=socket(PF_INET,SOCK_STREAM,0);
	udp_sock=socket(PF_INET,SOCK_DGRAM,0);
	printf("SOCK_STREAM: %d \n",SOCK_STREAM);//输出创建TCP、UDP套接字时传入的SOCK_STREAM和SOCK_DGRAM
	printf("SOCK_DGRAM: %d \n",SOCK_DGRAM);

	state=getsockopt(tcp_sock,SOL_SOCKET,SO_TYPE,(void*)&sock_type,&optlen);//获取套接字类型信息，如果是TCP套接字，那么sock_type值为SOCK_STREAM常数值1
	if(state){
		error_handling("getsockopt() error");
	}
	printf("Socket type one: %d \n",sock_type);

	state=getsockopt(udp_sock,SOL_SOCKET,SO_TYPE,(void*)&sock_type,&optlen);//获取套接字类型信息，如果是UDP套接字，那么sock_type值为SOCK_DGRAM常数值2
	if(state){
		error_handling("getsockopt() error");
	}
	printf("Socket type two: %d \n",sock_type);
	return 0;
}