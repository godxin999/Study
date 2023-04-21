/*************************************************************************
	> File Name: waitpid.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/21 10:28:42
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc,char **argv){

	int status;
	pid_t pid=fork();

	if(pid==0){
		sleep(15);//延迟子进程的执行
		return 24;
	}
	else{
		while(!waitpid(-1,&status,WNOHANG)){//向第三个参数传递WNOHANG，则如果之前没有终止的子进程将会返回0
			sleep(3);
			puts("sleep 3s");
		}

		if(WIFEXITED(status)){
			printf("Child send %d \n",WEXITSTATUS(status));
		}
	}
	return 0;
}