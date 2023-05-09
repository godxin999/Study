/*************************************************************************
	> File Name: echo_mpserv.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/9 8:35:38
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

void read_childproc(int sig){
	pid_t pid;
	int status;
	pid=waitpid(-1,&status,WNOHANG);
	printf("removed proc id: %d \n",pid);
}

int main(int argc,char **argv){

	int serv_sock,clnt_sock;
	struct sockaddr_in serv_adr,clnt_adr;

	pid_t pid;
	struct sigaction act;
	socklen_t adr_sz;
	int str_len,state;
	char buf[BUF_SIZE];
	if(argc!=2){
		printf("Usag : %s <port>\n",argv[0]);
		exit(1);
	}

	act.sa_handler=read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	state=sigaction(SIGCHLD,&act,0);
	serv_sock=socket(PF_INET,SOCK_STREAM,0);
	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1){
		error_handling("bind() error");
	}
	if(listen(serv_sock,5)==-1){
		error_handling("listen() error");
	}

	while(1){
		adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&adr_sz);
		if(clnt_sock==-1){
			continue;
		}
		else{
			puts("new client connected...");
		}
		pid=fork();//父子进程分别带有accept创建的套接字
		if(pid==-1){
			close(clnt_sock);
			continue;
		}
		if(pid==0){
			close(serv_sock);//关闭传递到子进程的服务器端套接字文件描述符
			while((str_len=read(clnt_sock,buf,BUF_SIZE))!=0){
				write(clnt_sock,buf,str_len);
			}
			close(clnt_sock);//关闭复制给子进程的套接字文件描述符
			puts("client disconnected...");
			return 0;
		}
		else{
			close(clnt_sock);
		}
	}
	close(serv_sock);//子进程和父进程都关闭才可以完全销毁套接字
	return 0;
}