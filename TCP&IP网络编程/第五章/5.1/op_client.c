/*************************************************************************
	> File Name: op_client.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/4 16:09:58
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4//结果的大小
#define OPSZ 4//Operand的大小
void error_handling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

int main(int argc,char **argv){
	int sock;
	char opmsg[BUF_SIZE];
	int result,opnd_cnt,i;
	struct sockaddr_in serv_adr;
	if(argc!=3){
		printf("Usage : %s <IP> <port>\n",argv[0]);
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
		puts("Connected.........");
	}

	fputs("Operand count: ",stdout);
	scanf("%d",&opnd_cnt);
	opmsg[0]=(char)opnd_cnt;//约定以1字节整数形式传递待运算整数的个数
	for(i=0;i<opnd_cnt;++i){
		printf("Operand %d: ",i+1);
		scanf("%d",(int*)&opmsg[i*OPSZ+1]);//约定以四字节形式传递运算数，转换为int*指针保存在char数组中
	}
	fgetc(stdin);//删除缓冲中的字符\n
	fputs("Operator: ",stdout);
	scanf("%c",&opmsg[opnd_cnt*OPSZ+1]);//输入运算符信息
	write(sock,opmsg,opnd_cnt*OPSZ+2);
	read(sock,&result,RLT_SIZE);//结果约定为4字节整数，故一次read调用即可接收

	printf("Operation result: %d \n",result);
	close(sock);
	return 0;
}