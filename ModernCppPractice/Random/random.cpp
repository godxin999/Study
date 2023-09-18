/*************************************************************************
	> File Name: random.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/17 22:23:06
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <numeric>
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
struct xorshift32{
	using result_type=uint32_t;
	xorshift32(size_t seed):seed(static_cast<result_type>(seed?seed:1)){}
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
	xorshift32 rng(std::random_device{}());
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
	xorshift32 rng(std::random_device{}());
	std::vector<std::string> vs={"hello","world","good","morning"};
	std::uniform_int_distribution<int> uni(0,vs.size()-1);
	for(int i=0;i<10;++i){
		std::cout<<vs[uni(rng)]<<std::endl;
	}
}
//正态分布
void test11(){
	xorshift32 rng{1};
	std::normal_distribution<double> nd(0,1);//均值为0，标准差为1
	for(int i=0;i<10;++i){
		std::cout<<nd(rng)<<std::endl;
	}
}
//向容器中添加随机数
void test12(){
	std::vector<int> vi(100);
	std::generate(vi.begin(),vi.end(),[rng=xorshift32(std::random_device{}()),uni=std::uniform_int_distribution<int>(0,100)]()mutable{
		return uni(rng);
	});
	for(const auto& i:vi){
		std::cout<<i<<" ";
	}
	std::cout<<std::endl;
}
void test13(){
	std::vector<int> vi;
	std::generate_n(std::back_inserter(vi),100,[rng=xorshift32(std::random_device{}()),uni=std::uniform_int_distribution<int>(0,100)]()mutable{
		return uni(rng);
	});
	for(const auto& i:vi){
		std::cout<<i<<" ";
	}
	std::cout<<std::endl;
}
//使用std::iota填充序列 并使用std::shuffle洗牌
void test14(){
	std::vector<int> vi(100);
	std::iota(vi.begin(),vi.end(),0);
	std::shuffle(vi.begin(),vi.end(),xorshift32(std::random_device{}()));
	for(const auto& i:vi){
		std::cout<<i<<" ";
	}
	std::cout<<std::endl;
}
//模拟抽卡
void test15(){
	std::vector<float> p={0.8,0.15,0.038,0.012};
	std::vector<float> p_scanned;
	std::inclusive_scan(p.begin(),p.end(),std::back_inserter(p_scanned));//inclusive_scan为并行std算法，且要求符号满足结合律
	std::vector<std::string> words={"R","SR","SSR","UR"};
	xorshift32 rng(std::random_device{}());
	std::uniform_real_distribution<float> unf(0,1);
	auto getWord=[&]()->std::string{
		float f=unf(rng);
		//std::cout<<f<<std::endl;
		auto it=std::upper_bound(p_scanned.begin(),p_scanned.end(),f);
		return words[it-p_scanned.begin()];
	};
	for(int i=0;i<10;++i){
		std::cout<<getWord()<<std::endl;
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
	//test10();
	//test11();
	//test12();
	//test13();
	//test14();
	test15();
}