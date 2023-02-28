#include "StrVec.h"
using namespace std;

allocator<string> StrVec::alloc;//类中只进行了声明，需要在类外进行定义

void StrVec::push_back(const string& s) {
	chk_n_alloc();
	cout << "copy" << endl;
	alloc.construct(first_free++, s);
}

void StrVec::push_back(string&& s) {
	chk_n_alloc();
	cout << "move" << endl;
	alloc.construct(first_free++, move(s));
}

pair<string*, string*> StrVec::alloc_n_copy(const string* b, const string* e) {
	auto data = alloc.allocate(e - b);//使用尾后指针减去首指针来确定分配的空间大小，并返回分配内存的开始位置
	return { data,uninitialized_copy(b,e,data) };//uninitialized_copy函数在data位置构造b-e的副本，并返回最后一个构造元素之后的位置
}

void StrVec::free() {
	if (elements) {//不能传递给deallocate一个空指针
		for (auto p = first_free; p != elements; ) {//逆序销毁元素
			alloc.destroy(--p);
		}
		alloc.deallocate(elements, cap - elements);
	}
}

StrVec::StrVec(const StrVec& s) {
	auto newdata = alloc_n_copy(s.begin(), s.end());//调用alloc_n_copy分配空间以s中一样的元素
	elements = newdata.first;
	first_free = cap = newdata.second;
}

StrVec::StrVec(StrVec&& s)noexcept :elements(s.elements),first_free(s.first_free),cap(s.cap) {//接管给定对象的内存
	s.elements = s.first_free = s.cap = nullptr;//将原有指针置空
}


StrVec::~StrVec() {
	free();
}

StrVec& StrVec::operator=(const StrVec& rhs) {
	auto data = alloc_n_copy(rhs.begin(), rhs.end());
	free();
	elements = data.first;
	first_free = cap = data.second;
	return *this;
}

StrVec& StrVec::operator=(StrVec&& rhs)noexcept {
	if (this != &rhs) {//直接检测自赋值
		free();//释放已有元素
		elements = rhs.elements;//接管资源
		first_free = rhs.first_free;
		cap = rhs.cap;
		rhs.elements = rhs.first_free = rhs.cap = nullptr;
	}
	return *this;
}

void StrVec::reallocate() {
	auto newcapacity = size() ? size() * 2 : 1;//分配两倍大小的内存空间
	/*
	auto newdata = alloc.allocate(newcapacity);//分配新内存
	auto dest = newdata;//指向新数组中下一个空闲位置
	auto elem = elements;//指向旧数组中下一个元素
	for (auto i = 0; i != size(); ++i) {//移动元素
		alloc.construct(dest++, std::move(*elem++));
	}
	free();//释放内存空间
	elements = newdata;//更新参数
	first_free = dest;
	cap = elements + newcapacity;
	*/
	auto first = alloc.allocate(newcapacity);
	auto last = uninitialized_copy(make_move_iterator(begin()), make_move_iterator(end()), first);//使用移动迭代器调用移动构造函数来构造元素
	free();
	elements = first;
	first_free = last;
	cap = elements + newcapacity;
}