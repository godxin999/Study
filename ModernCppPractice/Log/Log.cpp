/*************************************************************************
	> File Name: Log.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/22 21:51:04
 ************************************************************************/

#include "Log.hpp"
using namespace mylog;


int main(){
	set_log_path("a.log");
	int n = 100;
	MYLOG_P(n);
	MYLOG_PH(n);
	mylog_trace("Hello, {}!", "Trace");
	set_log_level(log_level::debug);
	mylog_debug("Hello, {}!", "Debug");

	return 0;
}