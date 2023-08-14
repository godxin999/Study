/*************************************************************************
	> File Name: main.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/14 11:26:27
 ************************************************************************/

#include <iostream>
#include "DarkRoast.hpp"
#include "Decaf.hpp"
#include "Espresso.hpp"
#include "HouseBlend.hpp"
#include "Mocha.hpp"
#include "Soy.hpp"
#include "Whip.hpp"
#include "Milk.hpp"

int main(){
	std::unique_ptr<Beverage> beverage = std::make_unique<Decaf>();
	beverage=std::make_unique<Mocha>(std::move(beverage));
	std::cout<<beverage->getDescription()<<"  $"<<beverage->cost()<<std::endl;

	std::unique_ptr<Beverage> b=std::make_unique<HouseBlend>();
	b=std::make_unique<Milk>(std::move(b));
	b=std::make_unique<Soy>(std::move(b));
	b=std::make_unique<Soy>(std::move(b));
	b=std::make_unique<Whip>(std::move(b));
	std::cout<<b->getDescription()<<"  $"<<b->cost()<<std::endl;
	return 0;
}