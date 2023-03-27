/*************************************************************************
	> File Name: fd_seri.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/27 17:12:00
 ************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

int main(){
	int fd1,fd2,fd3;
	//创建一个文件和两个套接字
	fd1=socket(PF_INET,SOCK_STREAM,0);
	fd2=open("test.dat",O_CREAT|O_WRONLY|O_TRUNC);
	fd3=socket(PF_INET,SOCK_DGRAM,0);
	//输出文件描述符
	printf("file descriptor 1: %d\n",fd1);
	printf("file descriptor 2: %d\n",fd2);
	printf("file descriptor 3: %d\n",fd3);
	close(fd1);
	close(fd2);
	close(fd3);
	return 0;
}