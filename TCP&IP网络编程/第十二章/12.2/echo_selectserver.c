/*************************************************************************
	> File Name: echo_selectserver.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/18 20:14:29
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100
void error_handling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

int main(int argc,char **argv){
	
	int serv_sock,clnt_sock;
	struct sockaddr_in serv_adr,clnt_adr;
	struct timeval timeout;
	fd_set reads,cpy_reads;

	socklen_t adr_sz;
	int fd_max,str_len,fd_num,i;
	char buf[BUF_SIZE];
	if(argc!=2){
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}
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

	FD_ZERO(&reads);
	FD_SET(serv_sock,&reads);//添加服务器端套接字为监视对象
	fd_max=serv_sock;

	while(1){
		cpy_reads=reads;
		timeout.tv_sec=5;
		timeout.tv_usec=5000;

		if((fd_num=select(fd_max+1,&cpy_reads,0,0,&timeout))==-1){
			break;
		}
		if(fd_num==0){
			continue;
		}

		for(i=0;i<fd_max+1;++i){//select函数返回值大于一时执行的循环
			if(FD_ISSET(i,&cpy_reads)){//查找有接收数据的套接字的文件描述符
				if(i==serv_sock){//如果是服务器套接字的变化，则受理连接请求
					adr_sz=sizeof(clnt_adr);
					clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_sock,&adr_sz);
					FD_SET(clnt_sock,&reads);//监视客户端套接字
					if(fd_max<clnt_sock){
						fd_max=clnt_sock;
					}
					printf("connected client: %d \n",clnt_sock);
				}
				else{//有要接收的数据
					str_len=read(i,buf,BUF_SIZE);
					if(str_len==0){//接收的数据为EOF
						FD_CLR(i,&reads);//解除监视
						close(i);//关闭套接字
						printf("closed client: %d \n",i);
					}
					else{
						write(i,buf,str_len);//执行回声服务
					}
				}
			}
		}
	}
	close(serv_sock);
	return 0;
}