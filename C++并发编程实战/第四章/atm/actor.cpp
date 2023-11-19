/*************************************************************************
	> File Name: actor.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/11/19 15:34:24
 ************************************************************************/

#include "message.h"
#include "withdraw_msg.h"
#include "atm.h"
#include "dispatcher.h"
#include "bank_machine.h"
#include "interface_machine.h"




int main() {
	bank_machine bank;
	interface_machine interface_hardware;
	atm machine(bank.get_sender(), interface_hardware.get_sender());
	std::thread bank_thread(&bank_machine::run, &bank);
	std::thread if_thread(&interface_machine::run, &interface_hardware);
	std::thread atm_thread(&atm::run, &machine);
	messaging::sender atmqueue(machine.get_sender());
	bool quit_pressed = false;
	while (!quit_pressed) {
		char c = getchar();
		switch (c) {
		case '0': {
			[[fallthrough]];
		}
		case '1': {
			[[fallthrough]];
		}
		case '2': {
			[[fallthrough]];
		}
		case '3': {
			[[fallthrough]];
		}
		case '4': {
			[[fallthrough]];
		}
		case '5': {
			[[fallthrough]];
		}
		case '6': {
			[[fallthrough]];
		}
		case '7': {
			[[fallthrough]];
		}
		case '8': {
			[[fallthrough]];
		}
		case '9': {
			atmqueue.send(digit_pressed(c));
			break;
		}
		case 'b': {
			atmqueue.send(balance_pressed());
			break;
		}
		case 'w': {
			atmqueue.send(withdraw_pressed(50));
			break;
		}
		case 'c': {
			atmqueue.send(cancel_pressed());
			break;
		}
		case 'q': {
			quit_pressed = true;
			break;
		}
		case 'i': {
			atmqueue.send(card_inserted("acc1234"));
			break;
		}
		}
	}
	bank.done();
	machine.done();
	interface_hardware.done();
	atm_thread.join();
	bank_thread.join();
	if_thread.join();

	return 0;
}