/*************************************************************************
	> File Name: select.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/18 18:55:43
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#define BUF_SIZE 3

int main(int argc,char **argv){
	fd_set reads,temps;
	int result,str_len;
	char buf[BUF_SIZE];
	struct timeval timeout;
	FD_ZERO(&reads);//初始化fd_set变量
	FD_SET(0,&reads);//将文件描述符0对应的位设置为1(监视标准输入)

	while(1){
		temps=reads;//记录初状态
		timeout.tv_sec=5;//每次在调用select函数前都要初始化timeval结构体的成员
		timeout.tv_usec=0;
		result=select(1,&temps,0,0,&timeout);//调用select函数，如果有控制台输入数据，则返回大于0的整数
		if(result==-1){
			puts("select() error");
			break;
		}
		else if(result==0){//没有文件描述符发生改变
			puts("timeout!");
		}
		else{
			if(FD_ISSET(0,&temps)){//验证发生变化的文件描述符是否是标准输入
				str_len=read(0,buf,BUF_SIZE);//从标准输入读取数据
				buf[str_len]=0;
				printf("message from console: %s",buf);
			}
		}
	}

	return 0;
}