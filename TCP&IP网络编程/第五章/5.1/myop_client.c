/*************************************************************************
	> File Name: op_client.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/4 8:27:42
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
	int sock;
	char message[BUF_SIZE];
	int str_len,i,ans;
	struct sockaddr_in serv_adr;

	if(argc!=3){
		exit(1);
	}

	sock=socket(PF_INET,SOCK_STREAM,0);
	if(sock==-1){
		error_handling("socket() error");
	}

	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));

	if(connect(sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1){
		error_handling("connect() error");
	}
	else{
		printf("Connected...........\n");
	}

	fputs("Operand count: ",stdout);
	fgets(message,BUF_SIZE,stdin);
	write(sock,message,strlen(message));
	int cnt=atoi(message);
	for(i=0;i<cnt;++i){
		printf("Operand %d: ",i+1);
		fgets(message,BUF_SIZE,stdin);
		write(sock,message,strlen(message));
	}
	printf("Operator: ");
	fgets(message,BUF_SIZE,stdin);
	write(sock,message,strlen(message));
	str_len=read(sock,message,BUF_SIZE-1);
	if(str_len==0){
		error_handling("read() error");
	}
	message[str_len]=0;
	printf("Operation result: ");
	ans=atoi(message);
	//printf("ans is %d\n",ans);
	//printf("len is %lu\n",strlen(message));
	for(i=0;i<strlen(message);++i){
		printf("%d",ans%10);
		ans/=10;
	}
	printf("\n");
	close(sock);
	return 0;
}