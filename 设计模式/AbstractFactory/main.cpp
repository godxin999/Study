/*************************************************************************
	> File Name: main.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/17 10:21:24
 ************************************************************************/

#include "NYPizzaStore.hpp"
#include "ChicagoPizzaStore.hpp"

int main(){
	std::unique_ptr<PizzaStore> NYStore = std::make_unique<NYPizzaStore>();
	std::unique_ptr<PizzaStore> ChicagoStore = std::make_unique<ChicagoPizzaStore>();

	std::unique_ptr<Pizza> pizza = NYStore->orderPizza("NYStylePizza");
	std::cout << "Ethan ordered a " << pizza->getName() << std::endl;
	std::unique_ptr<Pizza> pizza2 = ChicagoStore->orderPizza("ChicagoStylePizza");
	std::cout << "Joel ordered a " << pizza2->getName() << std::endl;
	return 0;
}