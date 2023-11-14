/*************************************************************************
	> File Name: condition_variable.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/11/14 20:06:29
 ************************************************************************/

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>

std::condition_variable cvA,cvB;
std::mutex mtx;
int num=1;

void test(){
	std::thread t1([](){
		for(;;){
			std::unique_lock<std::mutex> lock(mtx);//加锁

			cvA.wait(lock,[](){//条件变量根据第二个参数(谓词)进行判断，如果返回false，线程挂起并释放锁，直到其他线程调用notify_one()或者notify_all()唤醒线程，
				return num==1;
			});//如果谓词返回true，则继续拥有锁进行下面的操作

			++num;
			std::cout<<"thread A print 1\n";
			cvB.notify_one();//通知线程B
		}
	});
	std::thread t2([](){
		for(;;){
			std::unique_lock<std::mutex> lock(mtx);
			cvB.wait(lock,[](){
				return num==2;
			});

			--num;
			std::cout<<"thread B print 2\n";
			cvA.notify_one();//通知线程A
		}
	});

	t1.join();
	t2.join();
}



int main(){
	test();


	return 0;
}