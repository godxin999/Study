/*************************************************************************
	> File Name: main.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/16 11:23:23
 ************************************************************************/

#include "NYPizzaStore.hpp"
#include "ChicagoPizzaStore.hpp"
#include <memory>

int main(){
	auto NYStore=std::make_unique<NYPizzaStore>();
	auto ChicagoStore=std::make_unique<ChicagoPizzaStore>();

	auto NYCheesePizza=NYStore->orderPizza("cheese");
	auto ChicagoCheesePizza=ChicagoStore->orderPizza("cheese");
	return 0;
}