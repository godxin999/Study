#pragma once
#include "message.h"
#include <format>
#include "dispatcher.h"
#include <mutex>

class interface_machine
{
	std::mutex   iom;
	messaging::receiver incoming;
public:
	void done() {
		get_sender().send(messaging::close_queue());
	}
	void run() {
		try {
			for (;;) {
				incoming.wait().handle<messaging::dispatcher, issue_money, std::function<void(const issue_money& msg)>>(
					[&](const issue_money& msg) {
						{
							std::lock_guard<std::mutex> lk(iom);
							std::cout << std::format("issuing {}\n", msg.amount);
						}
					}, "issue_money"
				).handle<display_insufficient_funds, std::function<void(const display_insufficient_funds& msg)>>(
					[&](const display_insufficient_funds& msg) {
						{
							std::lock_guard<std::mutex> lk(iom);
							std::cout << "insufficient funds\n";
						}
					}, "display_insufficient_funds"
				).handle<display_enter_pin, std::function<void(const display_enter_pin& msg)>>(
					[&](const display_enter_pin& msg) {
						{
							std::lock_guard<std::mutex> lk(iom);
							std::cout << "Please enter your PIN (0-9)\n";
						}
					}, "display_enter_pin"
				).handle<display_enter_card, std::function<void(const display_enter_card& msg)>>(
					[&](const display_enter_card& msg) {
						{
							std::lock_guard<std::mutex> lk(iom);
							std::cout << "Please enter your card (I)\n";
						}
					}, "display_enter_card"
				).handle<display_balance, std::function<void(const display_balance& msg)>>(
					[&](const display_balance& msg) {
						{
							std::lock_guard<std::mutex> lk(iom);
							std::cout << std::format("The balance of your account is {}.\n", msg.amount);
						}
					}, "display_balance"
				).handle<display_withdrawal_options, std::function<void(const display_withdrawal_options& msg)>>(
					[&](const display_withdrawal_options& msg) {
						{
							std::lock_guard<std::mutex> lk(iom);
							std::cout << "Withdraw 50? (w)\n";
							std::cout << "Display Balance? (b)\n";
							std::cout << "Cancel? (c)\n";
						}
					}, "display_withdrawal_options"
				).handle<display_withdrawal_cancelled, std::function<void(const display_withdrawal_cancelled& msg)>>(
					[&](const display_withdrawal_cancelled& msg) {
						{
							std::lock_guard<std::mutex> lk(iom);
							std::cout << "Withdrawal cancelled\n";
						}
					}, "display_withdrawal_cancelled"
				).handle<display_pin_incorrect_message, std::function<void(const display_pin_incorrect_message& msg)>>(
					[&](const display_pin_incorrect_message& msg) {
						{
							std::lock_guard<std::mutex> lk(iom);
							std::cout << "PIN incorrect\n";
						}
					}, "display_pin_incorrect_message"
				).handle<eject_card, std::function<void(const eject_card& msg)>>(
					[&](const eject_card& msg) {
						{
							std::lock_guard<std::mutex> lk(iom);
							std::cout << "Ejecting card\n";
						}
					}, "eject_card"
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