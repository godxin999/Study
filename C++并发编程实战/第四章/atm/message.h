#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>

namespace messaging {
	//消息基类，队列中存储的是消息基类的指针
	struct message_base {
		virtual ~message_base() {}
	};
	//模板子类，每种消息都有特化类型
	template <typename Msg>
	struct wrapped_message :message_base {
		Msg contents;
		explicit wrapped_message(const Msg& contents_) :contents(contents_) {}
	};

	class queue {
	private:
		std::mutex m;
		std::condition_variable c;
		std::queue<std::shared_ptr<message_base>> q;
	public:
		template <typename T>
		void push(const T& msg) {
			std::lock_guard<std::mutex> lk(m);
			//包装发布的消息，并存储相关的指针
			q.push(std::make_shared<wrapped_message<T>>(msg));
			c.notify_all();
		}

		std::shared_ptr<message_base> wait_and_pop() {
			std::unique_lock<std::mutex> lk(m);
			//队列为空时阻塞等待
			c.wait(lk, [&] {
				return !q.empty();
				});
			auto res = q.front();
			q.pop();
			return res;
		}
	};
}