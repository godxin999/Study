/*************************************************************************
	> File Name: op_server.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/4 8:27:37
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
	int str_len,cnt,num,i;
	struct sockaddr_in serv_adr,clnt_adr;
	socklen_t clnt_adr_sz;

	if(argc!=2){
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
	
	if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1){
		error_handling("bind() error");
	}
	if(listen(serv_sock,5)==-1){
		error_handling("listen() error");
	}

	clnt_adr_sz=sizeof(clnt_adr);

	clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);
	if(clnt_sock==-1){
		error_handling("accept() error");
	}
	else{
		printf("Successfully connected");
	}

	str_len=read(clnt_sock,message,BUF_SIZE);
	if(str_len==0){
		error_handling("read() error");
	}
	else{
		cnt=atoi(message);
	}
	int *v=(int *)malloc(sizeof(int)*cnt);
	for(i=0;i<cnt;++i){
		str_len=read(clnt_sock,message,BUF_SIZE);
		if(str_len==0){
			error_handling("read error");
		}
		else{
			v[i]=atoi(message);
		}
	}
	char op;
	str_len=read(clnt_sock,message,BUF_SIZE);
	if(str_len==0){
		error_handling("read() error");
	}
	else{
		op=message[0];
	}
	int ans=v[0];
	if(op=='+'){
		for(int i=1;i<cnt;++i){
			ans+=v[i];
		}
	}
	else if(op=='-'){
		for(int i=1;i<cnt;++i){
			ans-=v[i];
		}
	}
	else if(op=='*'){
		for(int i=1;i<cnt;++i){
			ans*=v[i];
		}
	}
	else{
		for(int i=1;i<cnt;++i){
			ans/=v[i];
		}
	}
	for(i=0;ans;++i){
		message[i]=(ans%10+'0');
		ans/=10;
	}
	message[i]=0;
	write(clnt_sock,message,strlen(message));
	close(clnt_sock);
	close(serv_sock);
	free(v);
	return 0;
}
