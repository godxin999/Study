/*************************************************************************
	> File Name: main.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/31 10:57:04
 ************************************************************************/

#include "MeatPizzaBuilder.hpp"
#include "VeggiePizzaBuilder.hpp"
#include "PizzaDirector.hpp"


int main(){
	PizzaDirector pizzaDirector(std::make_unique<MeatPizzaBuilder>());
	auto pizza=pizzaDirector.makePizza();
	pizza->prepare();
	pizza->bake();
	pizza->cut();
	pizza->box();
	pizza->printDetail();

	pizzaDirector.setPizzaBuilder(std::make_unique<VeggiePizzaBuilder>());
	pizza=pizzaDirector.makePizza();
	pizza->prepare();
	pizza->bake();
	pizza->cut();
	pizza->box();
	pizza->printDetail();

	return 0;
}