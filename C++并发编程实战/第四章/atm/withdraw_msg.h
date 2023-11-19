#pragma once
#include "dispatcher.h"


//ȡ��
struct withdraw {
	std::string account;
	unsigned amount;
	//atm��Ϣ����
	mutable messaging::sender atm_queue;
	withdraw(const std::string& account_, unsigned amount_, messaging::sender atm_queue_) :account(account_), amount(amount_), atm_queue(atm_queue_) {}
};
//ȡ��ɹ�
struct withdraw_ok {};
//ȡ��ܾ�
struct withdraw_denied {};
//ȡ��ȡ��
struct cancel_withdrawal {
	std::string account;
	unsigned amount;
	cancel_withdrawal(const std::string& account_, unsigned amount_) :account(account_), amount(amount_) {}
};
//ȡ���Դ���
struct withdraw_processed {
	std::string account;
	unsigned amount;
	withdraw_processed(const std::string& account_, unsigned amount_) :account(account_), amount(amount_) {}
};
//�忨
struct card_inserted {
	std::string account;
	explicit card_inserted(const std::string& account_) :account(account_) {}
};
//�������ּ�
struct digit_pressed {
	char digit;
	explicit digit_pressed(char digit_) :digit(digit_) {}
};
//�����һ�ε��
struct clear_last_pressed {};
//������
struct eject_card {};
//���ȡ��
struct withdraw_pressed {
	unsigned amount;
	explicit withdraw_pressed(unsigned amount_) :amount(amount_) {}
};
//���ȡ��
struct cancel_pressed {};
//��Ǯ
struct issue_money {
	unsigned amount;
	explicit issue_money(unsigned amount_) :amount(amount_) {}
};
//��������
struct verify_pin {
	std::string account;
	std::string pin;
	mutable messaging::sender atm_queue;
	verify_pin(const std::string& account_, const std::string& pin_, messaging::sender atm_queue_) :account(account_), pin(pin_), atm_queue(atm_queue_) {}
};
//����������
struct pin_verified {};
//���벻��ȷ
struct pin_incorrect {};
//չʾ�������룬��ʾ�û���������
struct display_enter_pin {};
//��ʾ������
struct display_enter_card {};
//��ʾ�ʽ���
struct display_insufficient_funds {};
//��ʾȡǮ��ȡ��
struct display_withdrawal_cancelled {};
//��ʾ������Ϣ
struct display_pin_incorrect_message {};
//��ʾȡǮѡ��
struct display_withdrawal_options {};
//��ȡ���
struct get_balance {
	std::string account;
	mutable messaging::sender atm_queue;
	get_balance(const std::string& account_, messaging::sender atm_queue_) :account(account_), atm_queue(atm_queue_) {}
};
//���
struct balance {
	unsigned amount;
	explicit balance(unsigned amount_) :amount(amount_) {}
};
//��ʾ���
struct display_balance {
	unsigned amount;
	explicit display_balance(unsigned amount_) :amount(amount_) {}
};
//����Ѱ���
struct balance_pressed {};
