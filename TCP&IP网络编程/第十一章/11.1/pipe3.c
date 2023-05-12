/*************************************************************************
	> File Name: pipe3.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/12 10:35:11
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#define BUF_SIZE 30

int main(int argc,char **argv){
	int fds1[2],fds2[2];
	char str1[]="Who are you?";
	char str2[]="Thank you for your message";
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds1),pipe(fds2);//创建两个管道
	pid=fork();
	if(pid==0){
		write(fds1[1],str1,sizeof(str1));
		read(fds2[0],buf,BUF_SIZE);
		printf("Child proc output: %s \n",buf);
	}
	else{
		read(fds1[0],buf,BUF_SIZE);
		printf("Parent proc output: %s \n",buf);
		write(fds2[1],str2,sizeof(str2));
		sleep(3);
	}
	return 0;
}