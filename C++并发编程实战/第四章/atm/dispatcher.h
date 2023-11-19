#pragma once
#include <iostream>
#include "message.h"

namespace messaging {
	//示意关闭队列的消息
	class close_queue {};

	class dispatcher {
	private:
		queue* q;
		bool chained;
		dispatcher(const dispatcher&) = delete;
		dispatcher& operator=(const dispatcher&) = delete;
		//允许TemplateDispatcher的实例访问内部数据
		template <typename Dispatcher, typename Msg, typename Func> friend class TemplateDispatcher;

		void wait_and_dispatch() {
			for (;;) {
				auto msg = q->wait_and_pop();
				dispatch(msg);
			}
		}
		//dispatch判断消息是否属于close_queue类型，如果是则抛出异常
		bool dispatch(const std::shared_ptr<message_base>& msg) {
			//转型失败返回空指针
			if (dynamic_cast<wrapped_message<close_queue>*>(msg.get())) {
				throw close_queue();
			}
			return false;
		}
	public:
		dispatcher(dispatcher&& other) :q(other.q), chained(other.chained) {
			//上游的消息分发者不会等待消息
			other.chained = true;
		}
		explicit dispatcher(queue* q_) :q(q_), chained(false) {}

		//根据TemplateDispatcher处理某种具体类型的消息
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
		queue* q;//消息队列的指针
	public:
		//默认方式构造的sender不包含队列
		sender() :q(nullptr) {}

		explicit sender(queue* q_) :q(q_) {}

		template <typename Message>
		void send(const Message& msg) {
			if (q) {
				//发送队列会向队列添加消息
				q->push(msg);
			}
		}
	};

	class receiver {
	private:
		queue q;//消息队列
	public:
		//receiver对象允许隐式转换为sender对象，前者拥有的队列被后者引用
		operator sender() {
			return sender(&q);
		}
		//队列上的等待行为会创建一个dispatcher对象
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
		//类模板TemplateDispatcher的各个实例之间互为友元类
		template <typename Dispatcher, typename OtherMsg, typename OtherFunc> friend class TemplateDispatcher;

		void wait_and_dispatch() {
			for (;;) {
				auto msg = q->wait_and_pop();
				//如果消息已经妥善处理，则跳出无限循环
				if (dispatch(msg)) {
					break;
				}
			}
		}

		bool dispatch(const std::shared_ptr<message_base>& msg) {
			//查验消息类型并调用相应的处理函数
			if (wrapped_message<Msg>* wrapper = dynamic_cast<wrapped_message<Msg>*>(msg.get())) {
				//正确处理则返回true
				f(wrapper->contents);
				return true;
			}
			else {
				//衔接前一个dispatcher，形成连锁调用
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

		//按衔接成链的方式引入更多处理函数
		template<typename OtherMsg,typename OtherFunc>
		TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc> handle(OtherFunc&& of, std::string info_msg) {
			//使用移动构造返回值，并将当前对象的指针传递给他，并把这个对象的chained成员设置为true，表示当前对象已经被衔接
			return TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc>(q, this, std::forward<OtherFunc>(of), info_msg);
		}

		~TemplateDispatcher()noexcept(false) {
			if (!chained) {
				wait_and_dispatch();
			}
		}
	};


}