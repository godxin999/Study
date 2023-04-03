/*************************************************************************
	> File Name: echo_server.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/3 9:11:31
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

int main(int argc,char **argv){
	int serv_sock,clnt_sock;
	char message[BUF_SIZE];
	int str_len,i;
	struct sockaddr_in serv_adr,clnt_adr;
	socklen_t clnt_adr_sz;

	if(argc!=2){
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}

	serv_sock=socket(PF_INET,SOCK_STREAM,0);
	if(serv_sock==-1){
		error_handling("socket() error");
	}

	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1){//该套接字用于接受连接请求
		error_handling("bind() error");
	}

	if(listen(serv_sock,5)==-1){
		error_handling("listen() error");
	}

	clnt_adr_sz=sizeof(clnt_adr);
	//为了处理五个客户端连接而添加的循环语句，共调用5次accept函数，依次向5个客户端提供服务
	for(int i=0;i<5;++i){
		clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);//accept函数产生用于数据I/O的套接字
		if(clnt_sock==-1){
			error_handling("accept() error");
		}
		else{
			printf("Connect client %d \n",i+1);
		}
		
		while((str_len=read(clnt_sock,message,BUF_SIZE))!=0){
			write(clnt_sock,message,str_len);//提供回声服务，原封不动地传输读取的字符串
		}

		close(clnt_sock);//关闭accept函数创建的套接字
	}
	close(serv_sock);
	return 0;
}