#pragma once
#include "dispatcher.h"
#include "withdraw_msg.h"
#include <functional>

class bank_machine {
public:
	messaging::receiver incoming;
	unsigned balance;
public:
	bank_machine() :balance(199) {}
	void done() {
		get_sender().send(messaging::close_queue());
	}
	void run() {
		try {
			for (;;) {
				incoming.wait().handle < messaging::dispatcher, verify_pin, std::function<void(const verify_pin& msg)>>(
					[&](const verify_pin& msg) {
						if (msg.pin == "666666") {
							msg.atm_queue.send(pin_verified());
						}
						else {
							msg.atm_queue.send(pin_incorrect());
						}
					}, "verify_pin"
				).handle<withdraw, std::function<void(const withdraw& msg)>>(
					[&, this](const withdraw& msg) {
						if (this->balance >= msg.amount) {
							msg.atm_queue.send(withdraw_ok());
							this->balance -= msg.amount;
						}
						else {
							msg.atm_queue.send(withdraw_denied());
						}
					}, "withdraw"
				).handle<get_balance, std::function<void(const get_balance& msg)>>(
					[&](const get_balance& msg) {
						msg.atm_queue.send(::balance(balance));
					}, "get_balance"
				).handle<withdraw_processed, std::function<void(const withdraw_processed& msg)>>(
					[&](const withdraw_processed& msg) {
						std::cout << "withdraw processed\n";
					}, "withdraw_processed"
				).handle<cancel_withdrawal, std::function<void(const cancel_withdrawal& msg)>>(
					[&](const cancel_withdrawal& msg) {
						std::cout << "cancel withdraw\n";
					}, "cancel_withdrawal"
				);
			}
		}
		catch (const messaging::close_queue&) {

		}
	}
	messaging::sender get_sender() {
		return incoming;
	}
};