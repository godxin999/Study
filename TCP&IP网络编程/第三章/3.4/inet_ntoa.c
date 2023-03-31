/*************************************************************************
	> File Name: inet_ntoa.c
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/3/31 16:02:29
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc,char **argv){
	struct sockaddr_in addr1,addr2;
	char *str_ptr;
	char str_arr[20];

	addr1.sin_addr.s_addr=htonl(0x1020304);
	addr2.sin_addr.s_addr=htonl(0x1010101);

	str_ptr=inet_ntoa(addr1.sin_addr);//传递ip地址信息给inet_ntoa函数，并返回字符串类型的ip地址
	strcpy(str_arr,str_ptr);//将字符串形式的ip地址复制到str_arr中
	printf("Dotted-Decimal notation1: %s \n",str_ptr);

	inet_ntoa(addr2.sin_addr);//再次调用函数
	printf("Dotted-Decimal notation2: %s \n",str_ptr);//指针指向的地址已经被覆盖了新的ip地址字符串
	printf("Dotted-Decimal notation3: %s \n",str_arr);
	return 0;
}