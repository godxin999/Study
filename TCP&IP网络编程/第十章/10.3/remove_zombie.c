/*************************************************************************
	> File Name: remove_zombie.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/9 8:04:34
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int sig){
	int status;
	pid_t id=waitpid(-1,&status,WNOHANG);//调用wait_pid函数，防止子进程变成僵尸进程
	if(WIFEXITED(status)){
		printf("Removed proc id: %d \n",id);
		printf("Child send: %d \n",WEXITSTATUS(status));
	}
}

int main(int argc,char **argv){

	pid_t pid;
	struct sigaction act;//注册SIGCHLD信号对应的处理器
	act.sa_handler=read_childproc;//子进程结束时调用这个函数
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	sigaction(SIGCHLD,&act,0);

	pid=fork();
	if(pid==0){
		puts("Hi~ I'm child process");
		sleep(10);
		return 12;
	}
	else{
		printf("Child proc id: %d \n",pid);
		pid=fork();
		if(pid==0){
			puts("Hi~ I'm child process");
			sleep(10);
			exit(24);
		}
		else{
			int i;
			printf("Child proc id: %d \n",pid);
			for(i=0;i<5;++i){//为了等待SIGCHLD信号，父进程共暂停五次，发生信号时，父进程将被唤醒，实际暂停时间不到25秒
				puts("wait...");
				sleep(5);
			}
		}
	}


	return  0;
}