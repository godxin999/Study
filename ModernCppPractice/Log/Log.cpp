/*************************************************************************
	> File Name: Log.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/22 21:51:04
 ************************************************************************/

#include "Log.hpp"
using namespace MyLog;


int main(){
	/*log(Level::Debug,"Hello, {}!", "Debug");

	log(Level::Info,"Hello, {}!", "Info");

	change_level(Level::Debug);

	log(Level::Debug,"Hello, {}!", "Debug");*/

	MyLog_info("Hello, {}!", "Info");

	//set_level(Level::debug);

	int n=100;
	MYLOG_P(n);
	MyLog_trace("Hello, {}!", "Trace");
	MyLog_debug("Hello, {}!", "Debug");
	return 0;
}