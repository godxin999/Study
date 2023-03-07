#pragma once
#include "Blob.h"

template <typename T>
class BlobPtr {
public:
	BlobPtr() :curr(0) {}
	BlobPtr(Blob<T>& a, std::size_t sz = 0) :wptr(a.data), curr(sz) {}
	T& operator*()const {
		auto p = check(curr, "dereference past end");
		return (*p)[curr];
	}
	//在类模板内简化类名的使用，可以直接使用模板名而不提供实参
	BlobPtr& operator++();
	BlobPtr& operator--();
	BlobPtr operator++(int);
	BlobPtr operator--(int);
private:
	std::shared_ptr<std::vector<T>> check(std::size_t, const std::string&)const;
	std::weak_ptr<std::vector<T>> wptr;
	std::size_t curr;
};

template <typename T>
BlobPtr<T>& BlobPtr<T>::operator++() {
	check(curr, "increment past end of BlobPtr");
	++curr;
	return *this;
}

template <typename T>
BlobPtr<T>& BlobPtr<T>::operator--() {
	--curr;
	check(-1, "decrement past end of BlobPtr");
	return *this;
}

template<typename T>
BlobPtr<T> BlobPtr<T>::operator++(int) {
	BlobPtr ret = *this;
	++* this;
	return ret;
}

template<typename T>
BlobPtr<T> BlobPtr<T>::operator--(int){
	BlobPtr ret = *this;
	--* this;
	return ret;
}

template <typename T>
std::shared_ptr<std::vector<T>> BlobPtr<T>::check(std::size_t i, const std::string& msg)const {
	auto ret = wptr.lock();
	if (!ret) {
		throw std::runtime_error("unbound BlobPtr");
	}
	if (i >= ret->size()) {
		throw std::out_of_range(msg);
	}
	return ret;
}
