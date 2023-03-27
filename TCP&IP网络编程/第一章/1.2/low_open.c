/*************************************************************************
	> File Name: low_open.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/27 16:48:04
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void error_handling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

int main(){
	int fd;
	char buf[]="Let's go!\n";
	fd=open("data.txt",O_CREAT|O_WRONLY|O_TRUNC);//创建文件，并且以只写打开，如果存在该文件则清除全部现有数据
	if(fd==-1){
		error_handling("open() error");
	}
	printf("file descriptor: %d \n",fd);

	if(write(fd,buf,sizeof(buf))==-1){//向fd中保存的文件描述符的文件传输buf中保存的数据
		error_handling("write() error");
	}
	close(fd);
	return 0;
}