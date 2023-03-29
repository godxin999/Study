/*************************************************************************
	> File Name: tcp_client.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/29 13:22:29
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

int main(int argc,char **argv){
	int sock;
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len=0;
	int idx=0,read_len=0;

	if(argc!=3){
		printf("Usage : %s <IP> <port>\n",argv[0]);
		exit(1);
	}

	sock=socket(PF_INET,SOCK_STREAM,0);//使用IPV4协议族，并采用面向连接的数据传输，符合这两个条件的协议只有IPPROTO_TCP，即TCP套接字
	if(sock==-1){
		error_handling("socket() error");
	}

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));

	if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1){
		error_handling("connect() error");
	}

	while(read_len=read(sock,&message[idx++],1)){//反复调用read函数，每次读取一个字节，如果read返回0则跳出函数
		if(read_len==-1){
			error_handling("read() error");
		}
		str_len+=read_len;//read_len的长度始终为1，所以str_len最终存放的是读取的字节总数
	}

	printf("Message from server : %s \n",message);
	printf("Function read call count: %d \n",str_len);
	close(sock);
	return 0;
}