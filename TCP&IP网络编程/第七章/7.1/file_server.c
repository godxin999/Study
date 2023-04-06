/*************************************************************************
	> File Name: file_server.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/6 9:57:00
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

int main(int argc,char **argv){
	int serv_sd,clnt_sd;
	FILE *fp;
	char buf[BUF_SIZE];
	int read_cnt;

	struct sockaddr_in serv_adr,clnt_adr;
	socklen_t clnt_adr_sz;

	if(argc!=2){
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}

	fp=fopen("file_server.c","rb");//打开文件以向客户端传输服务端源文件file_server.c
	serv_sd=socket(PF_INET,SOCK_STREAM,0);

	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	bind(serv_sd,(struct sockaddr*)&serv_adr,sizeof(serv_adr));
	listen(serv_sd,5);

	clnt_adr_sz=sizeof(clnt_adr);
	clnt_sd=accept(serv_sd,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);

	while(1){
		read_cnt=fread((void*)buf,1,BUF_SIZE,fp);//每次从文件中读取BUF_SIZE个1字节
		if(read_cnt<BUF_SIZE){
			write(clnt_sd,buf,read_cnt);
			break;
		}
		write(clnt_sd,buf,BUF_SIZE);
	}

	shutdown(clnt_sd,SHUT_WR);//对输出流进行半关闭，向客户端传输了EOF，使客户端知道文件传输已完成
	read(clnt_sd,buf,BUF_SIZE);//因为只关闭了输出流，所以还能正常通过输入流收数据
	printf("Message from client: %s \n",buf);

	fclose(fp);
	close(clnt_sd);
	close(serv_sd);
	return 0;
}