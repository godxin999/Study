#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>

namespace messaging {
	//��Ϣ���࣬�����д洢������Ϣ�����ָ��
	struct message_base {
		virtual ~message_base() {}
	};
	//ģ�����࣬ÿ����Ϣ�����ػ�����
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
			//��װ��������Ϣ�����洢��ص�ָ��
			q.push(std::make_shared<wrapped_message<T>>(msg));
			c.notify_all();
		}

		std::shared_ptr<message_base> wait_and_pop() {
			std::unique_lock<std::mutex> lk(m);
			//����Ϊ��ʱ�����ȴ�
			c.wait(lk, [&] {
				return !q.empty();
				});
			auto res = q.front();
			q.pop();
			return res;
		}
	};
}