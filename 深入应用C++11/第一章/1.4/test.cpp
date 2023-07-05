/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/7/4 15:21:38
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <set>
#include <map>
using namespace std;
//采用const auto&减少损耗
void test1(){
	vector<string> vs(100,"123");
	for(const auto& str:vs){
		cout<<str<<endl;
	}
}
//auto推导出的是容器中的value_type
void test2(){
	set<int> ss={1,2,3};
	for(auto &val:ss){
		//cout<<val++<<endl;//错误，val为只读的，auto&会被推导为const int&
	}
	map<int,int> mii={{1,1},{2,2}};
	for(auto &val:mii){
		//cout<<val.first++<<endl;//错误，value_type中first为只读的
	}
}
//冒号后的表达式只被执行一次
vector<int> arr={1,2,3,4,5};
vector<int>& get_range(){
	cout<<"get_range ->: "<<endl;
	return arr;
}
void test3(){
	for(auto&val : get_range()){
		cout<<val<<endl;
	}
}
//在迭代时修改容器会导致迭代器失效
void test4(){
	vector<int> v={1,2,3,4,5};
	for(auto i:v){
		cout<<i<<" ";//g++:1 495 344392016 495 5 clang:1 2 3 4 5
		v.push_back(0);
	}
	cout<<endl;
}
//对区间进行迭代
namespace detail_range{
	template<typename T>
	class iterator{
	public:
		using value_type = T;
		using size_type = size_t;
	private:
		size_type cursor_;//位置
		const value_type step_;//步长
		value_type value_;//值
	public:
		iterator(size_type cur_start,value_type begin_val,value_type step_val):cursor_(cur_start), step_(step_val),value_(begin_val){
			value_+=(step_*cursor_);
		}
		value_type operator*() const{//在range-for中会对迭代器进行解引用
			return value_;
		}
		bool operator!=(const iterator &rhs)const{
			return (cursor_ != rhs.cursor_);
		}
		iterator& operator++(){
			value_+=step_;
			++cursor_;
			return *this;
		}
	};

	template <typename T>
	class impl{
	public:
		using value_type = T;
		using reference = const value_type&;
		using const_reference=const value_type&;
		using iterator=const detail_range::iterator<value_type>;
		using const_iterator=const detail_range::iterator<value_type>;
		using size_type = typename iterator::size_type;
	private:
		const value_type begin_;
		const value_type end_;
		const value_type step_;
		const size_type max_count_;

		size_type get_adjusted_count()const{
			if(step_>0&&begin_>=end_){
				throw std::logic_error("End vlaue must be greater than begin value");
			}
			else if(step_<0&&begin_ <= end_){
				throw std::logic_error("End value must be less than begin value");
			}
			size_type x=static_cast<size_type> ((end_ - begin_)/step_);
			if(begin_+(step_*x)!=end_){
				++x;
			}
			return x;
		}
	public:
		impl(value_type begin_val,value_type end_val,value_type step_val):begin_(begin_val), end_(end_val), step_(step_val),max_count_(get_adjusted_count()){}
		size_type size() const{
			return max_count_;
		}
		//留出begin、end函数供range-for语法使用，返回一个迭代器类型
		const_iterator begin() const{
			return {0,begin_,step_};
		}
		const_iterator end() const{
			return {max_count_,begin_,step_};
		}
	};
}
template <typename T>
detail_range::impl<T> range(T begin,T end){
	return {begin,end,1};
}
template <typename T>
detail_range::impl<T> range(T end){
	return {{},end,1};
}
template<typename T,typename U>
auto range(T begin,T end,U step)->detail_range::impl<decltype(begin+step)>{
	using r_t=detail_range::impl<decltype(begin + step)>;
	return r_t(begin,end,step);
}
void test5(){
	cout<<"range(15)"<<endl;
	for(auto i:range(15)){
		cout<<i<<" ";
	}
	cout<<endl;
	cout<<"range(2,6)"<<endl;
	for(auto i:range(2,6)){
		cout<<i<<" ";
	}
	cout<<endl;
	cout<<"range(2,6,3)"<<endl;
	for (auto i : range(2,6,3)) {
		cout<<i<<" ";
	}
	cout<<endl;
	cout<<"range(-2,-6,-3)"<<endl;
	for (auto i : range(-2,-6,-3)) {
		cout<<i<<" ";
	}
	cout<<endl;
	cout<<"range(2,8,0.5)"<<endl;
	for (auto i : range(2,8,0.5)) {
		cout<<i<<" ";//2 2.5 3 3.5 4 4.5 5 5.5 6 6.5 7 7.5
	}
	cout<<endl;
}
int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	test5();


}