/*************************************************************************
	> File Name: wait.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/21 10:17:41
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc,char **argv){
	int status;
	pid_t pid=fork();

	if(pid==0){
		return 3;//return语句终止子进程
	}
	else{
		printf("Child PID: %d \n",pid);
		pid=fork();
		if(pid==0){
			exit(7);//exit函数终止子进程
		}
		else{
			printf("Child PID: %d \n",pid);
			wait(&status);//将之前终止的子进程的相关信息保存到status变量，同时相关的子进程被完全销毁
			if(WIFEXITED(status)){//WIFEXITED宏验证子进程是否正常终止
				printf("Child send one: %d \n",WEXITSTATUS(status));//如果正常退出，则调用WEXITSTATUS宏输出子进程的返回值
			}
			wait(&status);//因为创建了两个子进程，所以再次调用wait函数
			if(WIFEXITED(status)){
				printf("Child send one: %d \n",WEXITSTATUS(status));
			}
			sleep(30);
		}
	}
	return 0;
}