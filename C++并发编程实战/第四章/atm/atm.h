#pragma once
#include "dispatcher.h"
#include <functional>
#include <iostream>

class atm {
private:
	messaging::sender bank;
	messaging::sender interface_hardware;
	messaging::receiver incoming;
	void (atm::* state)();//atm类成员函数的指针
	std::string account;
	unsigned withdrawal_amount;
	std::string pin;

	void process_withdrawal() {
		incoming.wait().handle<messaging::dispatcher, withdraw_ok, std::function<void(const withdraw_ok& msg)>>(
			[&](const withdraw_ok& msg) {
				interface_hardware.send(issue_money(withdrawal_amount));
				bank.send(withdraw_processed(account, withdrawal_amount));
				state = &atm::wait_for_action;
			}, "withdraw_ok"
		).handle<withdraw_denied, std::function<void(const withdraw_denied& msg)>>(
			[&](const withdraw_denied& msg) {
				interface_hardware.send(display_insufficient_funds());
				state = &atm::wait_for_action;
			}, "withdraw_denied"
		).handle<cancel_pressed, std::function<void(const cancel_pressed& msg)>>(
			[&](const cancel_pressed& msg) {
				bank.send(cancel_withdrawal(account, withdrawal_amount));
				interface_hardware.send(display_withdrawal_cancelled());
				state = &atm::done_processing;
			}, "cancel_pressed"
		);
	}

	void process_balance() {
		incoming.wait().handle<messaging::dispatcher, balance, std::function<void(const balance& msg)>>(
			[&](const balance& msg) {
				interface_hardware.send(display_balance(msg.amount));
				state = &atm::wait_for_action;
			}, "balance"
		).handle<cancel_pressed, std::function<void(const cancel_pressed& msg)>>(
			[&](const cancel_pressed& msg) {
				state = &atm::done_processing;
			}, "cancel_pressed"
		);
	}

	void wait_for_action() {
		interface_hardware.send(display_withdrawal_options());
		incoming.wait().handle<messaging::dispatcher, withdraw_pressed, std::function<void(const withdraw_pressed& msg)>>(
			[&](const withdraw_pressed& msg) {
				withdrawal_amount = msg.amount;
				bank.send(withdraw(account, msg.amount, incoming));
				state = &atm::process_withdrawal;
			}, "withdraw_pressed"
		).handle<balance_pressed, std::function<void(const balance_pressed& msg)>>(
			[&](const balance_pressed& msg) {
				bank.send(get_balance(account, incoming));
				state = &atm::process_balance;
			}, "balance_pressed"
		).handle<cancel_pressed, std::function<void(const cancel_pressed& msg)>>(
			[&](const cancel_pressed& msg) {
				state = &atm::done_processing;
			}, "cancel_pressed"
		);
	}

	void verifying_pin() {
		incoming.wait().handle<messaging::dispatcher, pin_verified, std::function<void(const pin_verified& msg)>>(
			[&](const pin_verified& msg) {
				state = &atm::wait_for_action;
			}, "pin_verified"
		).handle<pin_incorrect, std::function<void(const pin_incorrect& msg)>>(
			[&](const pin_incorrect& msg) {
				interface_hardware.send(display_pin_incorrect_message());
				state = &atm::done_processing;
			}, "pin_incorrect"
		).handle<cancel_pressed, std::function<void(const cancel_pressed& msg)>>(
			[&](const cancel_pressed& msg) {
				state = &atm::done_processing;
			}, "cancel_pressed"
		);
	}

	void getting_pin() {
		incoming.wait().handle<messaging::dispatcher, digit_pressed, std::function<void(const digit_pressed& msg)>>(
			[&](const digit_pressed& msg) {
				const auto pin_length = 6;
				pin += msg.digit;
				if (pin.length() == pin_length) {
					bank.send(verify_pin(account, pin, incoming));
					state = &atm::verifying_pin;
				}
			}, "digit_pressed"
		).handle<clear_last_pressed, std::function<void(const clear_last_pressed& msg)>>(
			[&](const clear_last_pressed& msg) {
				if (!pin.empty()) {
					pin.pop_back();
				}
			}, "clear_last_pressed"
		).handle<cancel_pressed, std::function<void(const cancel_pressed& msg)>>(
			[&](const cancel_pressed& msg) {
				state = &atm::done_processing;
			}, "cancel_pressed"
		);
	}

	void waiting_for_card() {
		interface_hardware.send(display_enter_card());
		incoming.wait().handle<messaging::dispatcher, card_inserted, std::function<void(const card_inserted& msg)>>(
			[&](const card_inserted& msg) {
				account = msg.account;
				pin = "";
				interface_hardware.send(display_enter_pin());
				state = &atm::getting_pin;
			}, "card_inserted"
		);
	}

	void done_processing() {
		interface_hardware.send(eject_card());
		state = &atm::waiting_for_card;
	}

	atm(const atm&) = delete;
	atm& operator=(const atm&) = delete;
public:
	atm(messaging::sender bank_, messaging::sender interface_hardware_) :bank(bank_), interface_hardware(interface_hardware_) {}

	void done() {
		get_sender().send(messaging::close_queue());
	}

	void run() {
		state = &atm::waiting_for_card;
		try {
			for (;;) {
				(this->*state)();
			}
		}
		catch (const messaging::close_queue&) {

		}
	}

	messaging::sender get_sender() {
		return incoming;
	}

};