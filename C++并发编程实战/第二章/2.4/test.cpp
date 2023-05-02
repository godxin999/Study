/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/5/2 17:00:46
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <thread>
#include <iterator>
#include <numeric>
#include <chrono>
using namespace std;
//hardware_concurrency()可以返回并发线程的数量
void test1(){
	cout<<thread::hardware_concurrency()<<endl;//16
}
//并发的accumulate
template <typename Iterator,typename T>
struct accumulate_block{
	void operator()(Iterator first,Iterator last,T &result){
		result=std::accumulate(first,last,result);
	}
};
template <typename Iterator,typename T>
T parallel_accumulate(Iterator first,Iterator last,T init){
	const unsigned long length=std::distance(first,last);
	if(!length){//如果输入的范围为空
		return init;//返回基值
	}

	const unsigned long min_per_thread=25;
	const unsigned long max_threads=(length+min_per_thread-1)/min_per_thread;//如果范围中元素多余一个，则需要用范围内元素总数除以线程中的最小任务数，从而确定启动线程的最大数量
	
	const unsigned long hardware_threads=std::thread::hardware_concurrency();
	
	const unsigned long num_threads=std::min(hardware_threads!=0?hardware_threads:2,max_threads);
	
	const unsigned long block_size=length/num_threads;//求出每个线程中处理的元素数量

	std::vector<T> results(num_threads);//存放中间结果
	std::vector<std::thread> threads(num_threads-1);//因为有主线程，所以数量为num_threads-1
	Iterator block_start=first;
	for(unsigned long i=0;i<(num_threads-1);++i){
		Iterator block_end=block_start;//block_end迭代器指向当前块的末尾
		std::advance(block_end,block_size);
		threads[i]=std::thread(accumulate_block<Iterator,T>(),block_start,block_end,std::ref(results[i]));//启动新线程来累加结果
		block_start=block_end;//当迭代器指向当前块末尾的时候，启动下一个块
	}
	accumulate_block<Iterator,T>()(block_start,last,results[num_threads-1]);//处理最终块的结果
	for(auto &entry:threads){
		entry.join();//汇入所有线程
	}
	return std::accumulate(results.begin(),results.end(),init);//将结果进行累加
}
using namespace std::chrono;
vector<int> v(100000000,1);
void test2(){
	long long ans=accumulate(v.begin(),v.end(),0);
}
void test3(){
	long long ans=parallel_accumulate(v.begin(),v.end(),0);
}

int main(){
	//test1();
	auto t1=steady_clock::now();
	test2();
	auto t2=steady_clock::now();
	auto dur=t2-t1;
	cout<<duration_cast<nanoseconds> (dur).count()<<endl;
	t1=steady_clock::now();
	test3();
	t2=steady_clock::now();
	dur=t2-t1;
	cout<<duration_cast<nanoseconds> (dur).count()<<endl;
}

