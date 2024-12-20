/*************************************************************************
	> File Name: echo_mpclient.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/9 9:12:41
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

void read_routine(int sock,char *buf){
	while(1){
		int str_len=read(sock,buf,BUF_SIZE);
		if(str_len==0){
			return ;
		}
		buf[str_len]=0;
		printf("Message from server: %s",buf);
	}
}
void write_routine(int sock,char *buf){
	while(1){
		fgets(buf,BUF_SIZE,stdin);
		if(!strcmp(buf,"q\n")||!strcmp(buf,"Q\n")){
			shutdown(sock,SHUT_WR);//向服务器端传递EOF，因为文件描述符被复制，所以需要shutdown函数另外传递
			return ;
		}
		write(sock,buf,strlen(buf));
	}
}

int main(int argc,char **argv){

	int sock;
	pid_t pid;
	char buf[BUF_SIZE];
	struct sockaddr_in serv_adr;
	if(argc!=3){
		printf("Usage : %s <IP> <port>\n",argv[0]);
		exit(1);
	}

	sock=socket(PF_INET,SOCK_STREAM,0);
	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));

	if(connect(sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1){
		error_handling("connect() error");
	}

	pid=fork();
	if(pid==0){
		write_routine(sock,buf);
	}
	else{
		read_routine(sock,buf);
	}
	close(sock);
	return 0;
}