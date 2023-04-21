/*************************************************************************
	> File Name: fork.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/21 9:59:41
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int gval=10;
int main(int argc,char **argv){
	pid_t pid;
	int lval=20;
	gval++,lval+=5;

	pid=fork();//创建子进程，子进程的pid为0，父进程的pid为子进程ID
	if(pid==0){
		gval+=2,lval+=2;
	}
	else{
		gval-=2;lval-=2;
	}

	if(pid==0){
		printf("Child Proc: [%d,%d] \n",gval,lval);
	}
	else{
		printf("Parent Proc: [%d,%d] \n",gval,lval);
	}

	return 0;
}