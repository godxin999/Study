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

template <typename T>
std::list<T> sequential_quick_sort(std::list<T> input){
	if(input.empty())return input;
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

void test_quick_sort(){
	std::list<int> input(100);
	std::generate(input.begin(),input.end(),[rng=std::mt19937(std::random_device{}()),uni=std::uniform_int_distribution<int>(0,10000)]()mutable{
		return uni(rng);
		});
	
	auto result=sequential_quick_sort(input);
}

template <typename T>
std::list<T> parallel_quick_sort(std::list<T> input){
	if(input.empty())return input;
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
	std::list<int> input(100);
	std::generate(input.begin(),input.end(),[rng=std::mt19937(std::random_device{}()),uni=std::uniform_int_distribution<int>(0,10000)]()mutable{
		return uni(rng);
		});
	
	auto result=parallel_quick_sort(input);
}

template <typename T>
std::list<T> thread_pool_quick_sort(std::list<T> input){
	if(input.empty())return input;
	std::list<T> result;

	result.splice(result.begin(),input,input.begin());

	const T& pivot=*result.begin();

	auto divide_point=std::partition(input.begin(),input.end(),[&](const T& t){
		return t<pivot;
	});

	std::list<T> lower_part;

	lower_part.splice(lower_part.end(),input,input.begin(),divide_point);
	auto new_lower=ThreadPool::instance().commit(&parallel_quick_sort<T>,std::move(lower_part));
	auto new_higher(parallel_quick_sort(std::move(input)));

	result.splice(result.end(),new_higher);
	result.splice(result.begin(),new_lower.get());
	return result;
}

void test_thread_pool_quick_sort(){
	std::list<int> input(100);
	std::generate(input.begin(),input.end(),[rng=std::mt19937(std::random_device{}()),uni=std::uniform_int_distribution<int>(0,10000)]()mutable{
		return uni(rng);
		});
	
	auto result=thread_pool_quick_sort(input);
}

int main(){
	//test_quick_sort();
	//test_parallel_quick_sort();
	test_thread_pool_quick_sort();


	return 0;
}