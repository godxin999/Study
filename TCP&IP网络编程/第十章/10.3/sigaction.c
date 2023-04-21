/*************************************************************************
	> File Name: sigaction.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/21 11:08:09
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig){
	if(sig==SIGALRM){
		puts("Time out!");
	}
	alarm(2);
}

int main(int argc,char **argv){
	int i;
	struct sigaction act;
	act.sa_handler=timeout;//将函数指针保存在sigaction结构体中
	sigemptyset(&act.sa_mask);//调用sigemptyset函数将sa_mask成员的所有位初始化为0
	act.sa_flags=0;//将sa_flags成员初始化为0
	sigaction(SIGALRM,&act,0);//注册SIGALRM信号的处理器

	alarm(2);

	for(int i=0;i<3;++i){
		puts("wait...");
		sleep(100);
	}
	return 0;
}