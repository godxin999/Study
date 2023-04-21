/*************************************************************************
	> File Name: zombie.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/21 10:08:44
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc,char **argv){

	pid_t pid=fork();

	if(pid==0){
		puts("Hi, I am a child process");
	}
	else{
		printf("Child Process ID: %d \n",pid);//输出子进程ID，可以通过该值查看子进程是否为僵尸进程
		sleep(30);//父进程暂停30秒，30秒后父进程和子进程将会同时销毁
	}

	if(pid==0){
		puts("End child process");
	}
	else{
		puts("End parent process");
	}
	return 0;
}