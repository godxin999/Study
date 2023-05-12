/*************************************************************************
	> File Name: pipe2.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/12 10:20:50
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#define BUF_SIZE 30

int main(int argc,char **argv){
	int fds[2];
	char str1[]="Who are you?";
	char str2[]="Thank you for your message";
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds);
	pid=fork();
	if(pid==0){
		write(fds[1],str1,sizeof(str1));
		sleep(2);
		read(fds[0],buf,BUF_SIZE);
		printf("Child proc output: %s \n",buf);
	}
	else{
		read(fds[0],buf,BUF_SIZE);//接受子进程在23行传输的数据
		printf("Parent proc output: %s \n",buf);
		write(fds[1],str2,sizeof(str2));//会被第25行的子进程接受
		sleep(3);
	}
	return 0;
}