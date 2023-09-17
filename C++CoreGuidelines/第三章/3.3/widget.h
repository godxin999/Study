/*************************************************************************
	> File Name: widget.h
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/16 19:57:02
 ************************************************************************/

//I.27 为了库的稳定，考虑使用PImpl惯用法

#include <memory>

class widget{
	class impl;
	std::unique_ptr<impl> pimpl;
public:
	void draw();
	widget(int);
	~widget();
	widget(widget&&)=default;
	widget& operator=(widget&&);
	widget(const widget&)=delete;
	widget& operator=(const widget&)=delete;

};