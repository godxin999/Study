/*************************************************************************
	> File Name: file_client.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/6 9:57:15
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

int main(int argc,char **argv){
	int sd;
	FILE *fp;

	char buf[BUF_SIZE];
	int read_cnt;
	struct sockaddr_in serv_adr;
	if(argc!=3){
		printf("Usage : %s <IP> <port>\n",argv[0]);
		exit(1);
	}

	fp=fopen("receive.dat","wb");//创建新文件以保存服务器传输的文件数据
	sd=socket(PF_INET,SOCK_STREAM,0);

	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));

	connect(sd,(struct sockaddr*)&serv_adr,sizeof(serv_adr));

	while((read_cnt=read(sd,buf,BUF_SIZE))!=0){
		fwrite((void*)buf,1,read_cnt,fp);//将读取的内容写到文件中
	}

	puts("Received file data");
	write(sd,"Thank you",10);//向服务端发送消息，若服务器未关闭输入流即可接受此消息
	fclose(fp);
	close(sd);
	return 0;
}