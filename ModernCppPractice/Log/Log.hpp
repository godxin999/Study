#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <format>
#include <source_location>
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <chrono>
#include <utility>

namespace mylog {
	//����foreach��
#define MYLOG_FOREACH_LEVEL(f)\
        f(trace) \
        f(debug) \
        f(info) \
        f(critical) \
        f(warning) \
        f(error) \
        f(fatal)
	//ʹ�ú�չ��Ϊö������
	enum class log_level : std::uint8_t {
#define _FUNCTION(name) name,
		MYLOG_FOREACH_LEVEL(_FUNCTION)
#undef _FUNCTION
	};
	//��װDetails�����ռ䣬���ٱ�¶�Ľӿ�
	namespace Details {
		//�Է�ģ�庯�����������������ֹ���ļ�����ʱ�����ظ�����
		inline std::string get_name_from_level(log_level level) {
			switch (level) {
#define _FUNCTION(name) case log_level::name: return #name;
				MYLOG_FOREACH_LEVEL(_FUNCTION)
#undef _FUNCTION
			}
			return "Unknown";
		}

		inline log_level get_level_from_name(std::string level) {
#define _FUNCTION(name) if(level==#name) return log_level::name;
			MYLOG_FOREACH_LEVEL(_FUNCTION)
#undef _FUNCTION
			return log_level::info;
		}
		//��װstd::format_string��std::source_location
		template <class T>
		struct with_source_location {
			T inner;
			std::source_location loc;
		public:
			//Ҫ���ṩ������U(const char[])���Թ�������T(std::format_string)
			template <class U> requires std::is_constructible_v<T, U>
			consteval with_source_location(U&& inner, std::source_location loc = std::source_location::current()) :inner(std::forward<U>(inner)), loc(std::move(loc)) {
				//consteval���캯����֤�ڱ���ʱ����
				//ʹ��Ĭ�ϲ�������ȡ��ǰλ��
			}
			constexpr const T& format()const {
				return inner;
			}
			constexpr const std::source_location& location()const {
				return loc;
			}
		};
		//$env:MYLOG_LEVEL="debug"
		inline log_level max_level = []()->log_level {
			//�������͵ص���
			//ʹ��inline��֤���ļ�Ψһ��
			//std::getenv����������ȫ
			if (auto name = std::getenv("MYLOG_LEVEL")) {
				return get_level_from_name(name);
			}
			return log_level::info;
		}();
		//$env:MYLOG_PATH="test.log"
		inline std::ofstream log_path = []()->std::ofstream {
			if (auto path = getenv("MYLOG_PATH")) {
				return std::ofstream(path, std::ios::app);//ʹ��std::ios::app׷��ģʽ
			}
			//return std::ofstream("a.log", std::ios::app);
			return std::ofstream();
		}();

		inline void output_log(log_level level, std::string msg, const std::source_location& loc) {
			std::chrono::zoned_time now{ std::chrono::current_zone(),std::chrono::system_clock::now() };//��ȡ��ʱ����ʱ��
			msg = std::format("{} {}:{} [{}] {}", now, loc.file_name(), loc.line(), Details::get_name_from_level(level), msg);//ʹ��std::format����ʹ��operator<<�������߳�����
			if (log_path) {//����������ļ���
				log_path << msg + '\n';
			}
			if (level >= max_level) {
				std::cout << msg + '\n';
			}
		}

		template <typename... Args>
		void mylog(log_level level, with_source_location<std::format_string<Args...>> fmt, Args&&... args) {
			const auto& loc = fmt.location();
			auto msg = std::vformat(fmt.format().get(), std::make_format_args(args...));//ʹ��std::vformat���ô���Ĳ�������msg
			output_log(level, std::move(msg), loc);
		}
	}

	inline void set_log_level(log_level level) {
		Details::max_level = level;
	}

	inline void set_log_path(std::string path) {
		Details::log_path = std::ofstream(path, std::ios::app);
	}
	//ʹ�ú�չ��Ϊ��Ӧ�����ģ�庯��
#define _FUNCTION(name) \
	template <typename... Args> \
	void mylog_##name(Details::with_source_location<std::format_string<Args...>> fmt,Args&&... args){ \
		return Details::mylog(log_level::name,std::move(fmt),std::forward<Args>(args)...); \
	}
	MYLOG_FOREACH_LEVEL(_FUNCTION)
#undef _FUNCTION

#define MYLOG_P(x) ::mylog::mylog_debug(#x " = {}",x)
#define MYLOG_PH(x) ::mylog::mylog_debug(#x " = {:#x}",x);

};

