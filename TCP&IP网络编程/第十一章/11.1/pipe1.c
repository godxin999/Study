/*************************************************************************
	> File Name: pipe1.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/12 9:58:48
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#define BUF_SIZE 30

int main(int argc,char **argv){

	int fds[2];//fds数组中保存用于I/O的文件描述符
	char str[]="Who are you?";
	char buf[BUF_SIZE];
	pid_t pid;
	
	pipe(fds);//使用pipe创建管道
	pid=fork();//父子进程同时拥有I/O文件描述符
	if(pid==0){
		write(fds[1],str,sizeof(str));//向管道传递字符串
	}
	else{
		read(fds[0],buf,BUF_SIZE);//从管道接受字符串
		puts(buf);
	}
	return 0;
}
