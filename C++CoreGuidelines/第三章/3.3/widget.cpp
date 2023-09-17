/*************************************************************************
	> File Name: widget.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/16 19:57:06
 ************************************************************************/

#include "widget.h"
#include <iostream>

class widget::impl{
	int n;
public:
	void draw(const widget& w){
		std::cout<<"draw "<<n<<std::endl;
	}
	impl(int n):n(n){

	}
};

void widget::draw(){
	pimpl->draw(*this);
}

widget::widget(int n):pimpl(std::make_unique<impl>(n)){

}

widget::~widget()=default;

widget& widget::operator=(widget&&)=default;


int main(){
	widget w(7);
	w.draw();


}