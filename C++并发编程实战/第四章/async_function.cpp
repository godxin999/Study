/*************************************************************************
	> File Name: async_function.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/11/16 15:25:41
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <list>
#include "thread_pool.h"

const int ConcurrentThreshold=512;

template <typename Iter>
void insertion_sort(Iter begin ,Iter end){
	if(distance(begin,end)<=1)return;
	for(auto it=begin;it!=end;++it){
		//first new_first end三个迭代器，将new_first成为新范围的第一个元素，将new_first-1成为新范围的最后一个元素
		//eg: 1 2 3 4 5 std::rotate(begin(),begin()+2,end())->3 4 5 1 2
		std::rotate(std::upper_bound(begin,it,*it),it,std::next(it));//upper_bound在有序部分中找到第一个大于等于当前元素的位置，将当前元素插入到upper_bound返回的位置(第一个大于等于它的元素)之前，next(it)尾后位置表示旋转的部分只有一个元素
	}
}

template <typename Iter>
Iter partition(Iter begin,Iter end){
	Iter mid=std::next(begin,std::distance(begin,end)/2);
	if(*mid<*begin)std::iter_swap(mid,begin);
	if(*end<*begin)std::iter_swap(end,begin);
	if(*end<*mid)std::iter_swap(mid,end);

	std::iter_swap(mid,std::prev(end));
	auto pivot=*(std::prev(end));

	auto divide_point=std::partition(begin,std::prev(end),[&](const auto& t){
		return t<pivot;
	});

	std::iter_swap(divide_point,std::prev(end));
	return divide_point;
}

template <typename Iter>
void sequential_quick_sort(Iter begin,Iter end){
	if(std::distance(begin,end)<=1)return;
	if(std::distance(begin,end)<16){
		insertion_sort(begin,end);
		return;
	}
	auto divide_point=partition(begin,end);
	sequential_quick_sort(begin,divide_point);
	sequential_quick_sort(std::next(divide_point),end);
}

template <typename Iter>
void parallel_quick_sort(Iter begin,Iter end){
	if(std::distance(begin,end)<=1)return;
	if(std::distance(begin,end)<ConcurrentThreshold){
		sequential_quick_sort(begin,end);
		return;
	}
	auto divide_point=partition(begin,end);
	auto future=std::async(&parallel_quick_sort<Iter>,begin,divide_point);
	parallel_quick_sort(std::next(divide_point),end);
	future.get();
}

/*
template <typename T>
std::list<T> sequential_quick_sort(std::list<T> input){
	if(input.empty()){
		return input;
	}
	std::list<T> result;
	//将input的第一个元素放入result中，并将其从input中删除
	result.splice(result.begin(),input,input.begin());
	//使用result的第一个元素切割input的列表
	const T& pivot=*result.begin();
	//将input中的元素分为两部分，一部分小于pivot，一部分大于pivot，返回分割点
	auto divide_point=std::partition(input.begin(),input.end(),[&](const T& t){
		return t<pivot;
	});

	std::list<T> lower_part;
	//将小于pivot的元素放入lower_part中，并从input中删除
	lower_part.splice(lower_part.end(),input,input.begin(),divide_point);
	//递归调用，对两部分进行排序
	auto new_lower(sequential_quick_sort(std::move(lower_part)));
	auto new_higher(sequential_quick_sort(std::move(input)));
	//将new_higher拼接到result的末尾，将new_lower拼接到result的开头
	result.splice(result.end(),new_higher);
	result.splice(result.begin(),new_lower);
	return result;
}

template <typename T>
std::list<T> parallel_quick_sort(std::list<T> input){
	if(input.empty()){
		return input;
	}
	std::list<T> result;

	result.splice(result.begin(),input,input.begin());

	const T& pivot=*result.begin();

	auto divide_point=std::partition(input.begin(),input.end(),[&](const T& t){
		return t<pivot;
	});

	std::list<T> lower_part;

	lower_part.splice(lower_part.end(),input,input.begin(),divide_point);
	auto new_lower(std::async(&parallel_quick_sort<T>,std::move(lower_part)));
	auto new_higher(parallel_quick_sort(std::move(input)));

	result.splice(result.end(),new_higher);
	result.splice(result.begin(),new_lower.get());
	return result;
}

void test_parallel_quick_sort(){
	std::list<int> input(1000);
	std::generate(input.begin(),input.end(),[&](){
		static std::mt19937 gen(std::random_device{}());
		static std::uniform_int_distribution<> dis(0,10000);
		return dis(gen);
	});
	auto start=std::chrono::steady_clock::now();
	auto result=parallel_quick_sort(input);
	auto end=std::chrono::steady_clock::now();
	std::cout<<std::chrono::duration<double,std::milli>(end-start).count()<<"ms\n";
}
*/
int main(){
	std::vector<int> input(10000);
	std::generate(input.begin(),input.end(),[&](){
		static std::mt19937 gen(std::random_device{}());
		static std::uniform_int_distribution<> dis(0,10000);
		return dis(gen);
	});
	auto now=std::chrono::steady_clock::now();
	parallel_quick_sort(input.begin(),input.end());
	auto end=std::chrono::steady_clock::now();
	std::cout<<std::chrono::duration<double,std::milli>(end-now).count()<<"ms\n";
	for(auto it=input.begin();it!=input.end();++it){
		std::cout<<*it<<" ";
	}



	return 0;
}