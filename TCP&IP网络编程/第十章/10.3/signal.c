/*************************************************************************
	> File Name: signal.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/4/21 10:52:38
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig){//信号处理函数
	if(sig==SIGALRM){
		puts("Time out!");
	}
	alarm(2);//每隔两秒重复产生SIGALRM信号
}
void keycontrol(int sig){//信号处理函数
	if(sig==SIGINT){
		puts("CRTL+C pressed");
	}
}

int main(int argc,char **argv){
	int i;
	signal(SIGALRM,timeout);//注册SIGALRM信号和信号处理函数
	signal(SIGINT,keycontrol);
	alarm(2);//2秒后产生SIGALRM信号

	for(int i=0;i<5;++i){
		puts("wait...");
		sleep(100);//产生信号时，会唤醒由于调用sleep函数而进入阻塞状态的进程，一旦唤醒便不会再次进入睡眠状态
	}
	return 0;
}