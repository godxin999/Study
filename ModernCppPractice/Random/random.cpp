/*************************************************************************
	> File Name: random.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/17 22:23:06
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
#include <vector>
#include <cstring>

void test1(){
	srand(time(NULL));//同一秒获得的种子相同，导致生成的随机数相同
	int num = rand()%10000;//msvc上RAND_MAX为32767，如果模的数较大，则随机分布不均匀
	std::cout<<num<<std::endl;
}

//使用std::mt19937随机数生成器，但是范围由生成器决定
void test2(){
	std::mt19937 rng;//random number generator
	std::cout<<"rng.min()="<<rng.min()<<" "<<"rng.max()="<<rng.max()<<std::endl;//rng.min()=0 rng.max()=4294967295
	size_t num=rng();
	std::cout<<num<<std::endl;
}
//使用std::uniform_int_distribution<int>自定义随机数范围
void test3(){
	std::mt19937 rng;//random number generator
	std::uniform_int_distribution<int> uni(0,100);
	size_t num=uni(rng);
	std::cout<<num<<std::endl;
}
//生成浮点随机数
void test4(){
	srand(time(NULL));
	float num = rand()/(float)RAND_MAX;//C语言生成随机0-1浮点数
	std::cout<<num<<std::endl;
}
//使用std::uniform_real_distribution<float>生成随机浮点数
void test5(){
	std::mt19937 rng;
	std::uniform_real_distribution<float> unf(0,1);
	float num=unf(rng);
	std::cout<<num<<std::endl;
}
//为随机数生成器指定种子
void test6(){
	std::mt19937 rng(42);//指定种子
	std::uniform_real_distribution<float> unf(0,1);
	float num=unf(rng);
	std::cout<<num<<std::endl;
}
//使用std::random_device生成随机种子
void test7(){
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_real_distribution<float> unf(0,1);
	float num=unf(rng);
	std::cout<<num<<std::endl;
}
//使用xorshift算法生成随机数
/*
struct xorshift32_state{
	uint32_t seed;
};
uint32_t xorshift32(struct xorshift32_state *state){
	uint32_t x=state->seed;//种子需要非零
	x^=x<<13;
	x^=x>>17;
	x^=x<<5;
	return state->seed=x;
}
void test8(){
	xorshift32_state rng{1};
	std::cout<<xorshift32(&rng)<<std::endl;
}
*/
//对xorshift32进行封装使其支持std::uniform_int_distribution
struct xorshift32_state{
	using result_type=uint32_t;
	xorshift32_state(size_t seed):seed(static_cast<result_type>(seed?seed:1)){}
	result_type seed=min();
	constexpr result_type operator()()noexcept{
		result_type x=seed;
		x^=x<<13;
		x^=x>>17;
		x^=x<<5;
		return seed=x;
	}
	static constexpr result_type min()noexcept{
		return std::numeric_limits<result_type>::min()+1;
	}
	static constexpr result_type max()noexcept{
		return std::numeric_limits<result_type>::max();
	}
};
void test9(){
	xorshift32_state rng(std::random_device{}());
	std::uniform_int_distribution<int> uni(0,100);
	for(int i=0;i<10;++i){
		std::cout<<uni(rng)<<std::endl;
	}
	std::uniform_real_distribution<float> unf(0,100);
	for(int i=0;i<10;++i){
		std::cout<<unf(rng)<<std::endl;
	}
}
void test10(){
	xorshift32_state rng(std::random_device{}());
	std::vector<std::string> vs={"hello","world","good","morning"};
	std::uniform_int_distribution<int> uni(0,vs.size()-1);
	for(int i=0;i<10;++i){
		std::cout<<vs[uni(rng)]<<std::endl;
	}
}

int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	//test6();
	//test7();
	//test8();
	//test9();
	test10();
}