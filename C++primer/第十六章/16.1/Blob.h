#pragma once
#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <memory>
template <typename> class BlobPtr;//模板声明后才可以声明友元
template <typename> class Blob;//声明模板以重载operator==
template <typename T> bool operator==(const Blob<T>&, const Blob<T>&);//模板声明后才可以声明友元

template <typename T>
class Blob {
public:
	//using value_type = T;
	//using size_type = std::vector<T>::size_type;
	friend class BlobPtr<T>;//友元要带模板参数
	friend bool operator==<T>(const Blob<T>&, const Blob<T>&);//模板参数在==之后
	typedef T value_type;
	typedef typename std::vector<T>::size_type size_type;
	Blob() :data(std::make_shared<std::vector<T>>()) {}
	Blob(std::initializer_list<T> il) :data(std::make_shared<std::vector<T>>(il)) {}
	size_type size()const {
		return data->size();
	}
	bool empty()const {
		return data->empty();
	}
	void push_back(const T& t) {
		data->push_back(t);
	}
	void push_back(T&& t) {
		data->push_back(std::move(t));
	}
	void pop_back();
	T& back();
	T& operator[](size_type i);
	const T& operator[](size_type i)const;
private:
	std::shared_ptr<std::vector<T>> data;
	void check(size_type i, const std::string& msg)const;
};

template <typename T>
void Blob<T>::check(size_type i, const std::string& msg)const {
	if (i >= data->size()) {
		throw std::out_of_range(msg);
	}
}

template <typename T>
T& Blob<T>::back() {
	check(0, "back on Blob");
	return data->back();
}

template <typename T>
T& Blob<T>::operator[](size_type i) {
	check(i, "subscript out of range");
	return (*data)[i];
}

template <typename T>
const T& Blob<T>::operator[](size_type i)const {
	check(i, "subscript out of range");
	return (*data)[i];
}

template <typename T>
void Blob<T>::pop_back() {
	check(0, "pop_back on empty Blob");
	data->pop_back();
}

template <typename T>
bool operator==(const Blob<T>& lhs, const Blob<T>& rhs) {
	if (lhs.size() != rhs.size())return false;
	for (std::size_t i = 0; i < lhs.size(); ++i) {
		if (lhs[i] != rhs[i])return false;
	}
	return true;
}

