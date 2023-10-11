/*************************************************************************
	> File Name: test.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/10/11 17:44:52
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstring>

//E.25 如果不能抛出异常，则模仿RAII进行资源管理
error_indicator func(std::string& arg){
	Gadget g{arg};
	if(!g.valid())return gadget_construction_error;//添加一个成员变量来模拟RAII
	//...
	return 0;//调用者虚测试func的返回值
}
//E.26 如果不能抛出异常，考虑快速失败
void f(int n){
	//...
	p=static_cast<X*>(malloc(n*sizeof(X)));
	if(!p)std::abort();
}
//E.27 如果不能抛出异常，请系统地使用错误代码
std::pair<int,error_indicator> user(){
	error_indicator err=0;

	Gadget g1=make_gadget(17);
	Gadget g2=make_gadget(17);

	if(!g1.valid()){
		err=g1_error;
		goto exit;
	}
	if(!g2.valid()){
		err=g2_error;
		goto exit;
	}
	if(all_foobar(g1,g2)){
		err=foobar_error;
		goto exit;
	}
	//...
exit:
	if(g1.valid())g1.cleanup();
	if(g2.valid())g2.cleanup();
	return {res,err}
}

int main(){




	return 0;
}