#pragma once
#include <iostream>
#include "message.h"

namespace messaging {
	//ʾ��رն��е���Ϣ
	class close_queue {};

	class dispatcher {
	private:
		queue* q;
		bool chained;
		dispatcher(const dispatcher&) = delete;
		dispatcher& operator=(const dispatcher&) = delete;
		//����TemplateDispatcher��ʵ�������ڲ�����
		template <typename Dispatcher, typename Msg, typename Func> friend class TemplateDispatcher;

		void wait_and_dispatch() {
			for (;;) {
				auto msg = q->wait_and_pop();
				dispatch(msg);
			}
		}
		//dispatch�ж���Ϣ�Ƿ�����close_queue���ͣ���������׳��쳣
		bool dispatch(const std::shared_ptr<message_base>& msg) {
			//ת��ʧ�ܷ��ؿ�ָ��
			if (dynamic_cast<wrapped_message<close_queue>*>(msg.get())) {
				throw close_queue();
			}
			return false;
		}
	public:
		dispatcher(dispatcher&& other) :q(other.q), chained(other.chained) {
			//���ε���Ϣ�ַ��߲���ȴ���Ϣ
			other.chained = true;
		}
		explicit dispatcher(queue* q_) :q(q_), chained(false) {}

		//����TemplateDispatcher����ĳ�־������͵���Ϣ
		template <typename Dispatcher, typename Msg, typename Func>
		TemplateDispatcher<Dispatcher, Msg, Func> handle(Func&& f, std::string info_msg) {
			return TemplateDispatcher<Dispatcher, Msg, Func>(q, this, std::forward<Func>(f), info_msg);
		}

		~dispatcher()noexcept(false) {
			if (!chained) {
				wait_and_dispatch();
			}
		}
	};

	class sender {
	private:
		queue* q;//��Ϣ���е�ָ��
	public:
		//Ĭ�Ϸ�ʽ�����sender����������
		sender() :q(nullptr) {}

		explicit sender(queue* q_) :q(q_) {}

		template <typename Message>
		void send(const Message& msg) {
			if (q) {
				//���Ͷ��л�����������Ϣ
				q->push(msg);
			}
		}
	};

	class receiver {
	private:
		queue q;//��Ϣ����
	public:
		//receiver����������ʽת��Ϊsender����ǰ��ӵ�еĶ��б���������
		operator sender() {
			return sender(&q);
		}
		//�����ϵĵȴ���Ϊ�ᴴ��һ��dispatcher����
		dispatcher wait() {
			return dispatcher(&q);
		}
	};

	template <typename PreviousDispatcher, typename Msg, typename Func>
	class TemplateDispatcher {
	private:
		queue* q;
		PreviousDispatcher* prev;
		Func f;
		bool chained;
		std::string _msg;
		TemplateDispatcher(const TemplateDispatcher&) = delete;
		TemplateDispatcher& operator=(const TemplateDispatcher&) = delete;
		//��ģ��TemplateDispatcher�ĸ���ʵ��֮�以Ϊ��Ԫ��
		template <typename Dispatcher, typename OtherMsg, typename OtherFunc> friend class TemplateDispatcher;

		void wait_and_dispatch() {
			for (;;) {
				auto msg = q->wait_and_pop();
				//�����Ϣ�Ѿ����ƴ�������������ѭ��
				if (dispatch(msg)) {
					break;
				}
			}
		}

		bool dispatch(const std::shared_ptr<message_base>& msg) {
			//������Ϣ���Ͳ�������Ӧ�Ĵ�����
			if (wrapped_message<Msg>* wrapper = dynamic_cast<wrapped_message<Msg>*>(msg.get())) {
				//��ȷ�����򷵻�true
				f(wrapper->contents);
				return true;
			}
			else {
				//�ν�ǰһ��dispatcher���γ���������
				return prev->dispatch(msg);
			}
		}
	public:
		TemplateDispatcher(TemplateDispatcher&& other) :q(other.q), prev(other.prev), f(std::move(other.f)), chained(other.chained), _msg(other._msg) {
			other.chained = true;
		}
		TemplateDispatcher(queue* q_, PreviousDispatcher* prev_, Func&& f_, std::string msg) :q(q_), prev(prev_), f(std::forward<Func>(f_)), chained(false), _msg(msg) {
			prev_->chained = true;
		}

		//���νӳ����ķ�ʽ������ദ����
		template<typename OtherMsg,typename OtherFunc>
		TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc> handle(OtherFunc&& of, std::string info_msg) {
			//ʹ���ƶ����췵��ֵ��������ǰ�����ָ�봫�ݸ�����������������chained��Ա����Ϊtrue����ʾ��ǰ�����Ѿ����ν�
			return TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc>(q, this, std::forward<OtherFunc>(of), info_msg);
		}

		~TemplateDispatcher()noexcept(false) {
			if (!chained) {
				wait_and_dispatch();
			}
		}
	};


}