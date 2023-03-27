/*************************************************************************
	> File Name: low_read.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/27 16:48:13
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define BUF_SIZE 100

void error_handling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

int main(){
	int fd;
	char buf[BUF_SIZE];

	fd=open("data.txt",O_RDONLY);//以只读模式打开文件
	if(fd==-1){
		error_handling("open() error");
	}
	printf("file descriptor: %d \n",fd);

	if(read(fd,buf,sizeof(buf))==-1){//使用read函数向buf中保存读入的数据
		error_handling("read() error");
	}
	printf("file data: %s",buf);
	close(fd);
	return 0;
}