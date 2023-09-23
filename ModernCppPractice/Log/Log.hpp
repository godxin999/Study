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
	//定义foreach宏
#define MYLOG_FOREACH_LEVEL(f)\
        f(trace) \
        f(debug) \
        f(info) \
        f(critical) \
        f(warning) \
        f(error) \
        f(fatal)
	//使用宏展开为枚举类型
	enum class log_level : std::uint8_t {
#define _FUNCTION(name) name,
		MYLOG_FOREACH_LEVEL(_FUNCTION)
#undef _FUNCTION
	};
	//封装Details命名空间，减少暴露的接口
	namespace Details {
		//对非模板函数和类进行内联，防止多文件链接时出现重复定义
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
		//封装std::format_string和std::source_location
		template <class T>
		struct with_source_location {
			T inner;
			std::source_location loc;
		public:
			//要求提供的类型U(const char[])可以构造类型T(std::format_string)
			template <class U> requires std::is_constructible_v<T, U>
			consteval with_source_location(U&& inner, std::source_location loc = std::source_location::current()) :inner(std::forward<U>(inner)), loc(std::move(loc)) {
				//consteval构造函数保证在编译时调用
				//使用默认参数来获取当前位置
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
			//声明并就地调用
			//使用inline保证多文件唯一性
			//std::getenv函数并不安全
			if (auto name = std::getenv("MYLOG_LEVEL")) {
				return get_level_from_name(name);
			}
			return log_level::info;
		}();
		//$env:MYLOG_PATH="test.log"
		inline std::ofstream log_path = []()->std::ofstream {
			if (auto path = getenv("MYLOG_PATH")) {
				return std::ofstream(path, std::ios::app);//使用std::ios::app追加模式
			}
			//return std::ofstream("a.log", std::ios::app);
			return std::ofstream();
		}();

		inline void output_log(log_level level, std::string msg, const std::source_location& loc) {
			std::chrono::zoned_time now{ std::chrono::current_zone(),std::chrono::system_clock::now() };//获取带时区的时间
			msg = std::format("{} {}:{} [{}] {}", now, loc.file_name(), loc.line(), Details::get_name_from_level(level), msg);//使用std::format避免使用operator<<带来的线程问题
			if (log_path) {//优先输出到文件中
				log_path << msg + '\n';
			}
			if (level >= max_level) {
				std::cout << msg + '\n';
			}
		}

		template <typename... Args>
		void mylog(log_level level, with_source_location<std::format_string<Args...>> fmt, Args&&... args) {
			const auto& loc = fmt.location();
			auto msg = std::vformat(fmt.format().get(), std::make_format_args(args...));//使用std::vformat利用传入的参数构造msg
			output_log(level, std::move(msg), loc);
		}
	}

	inline void set_log_level(log_level level) {
		Details::max_level = level;
	}

	inline void set_log_path(std::string path) {
		Details::log_path = std::ofstream(path, std::ios::app);
	}
	//使用宏展开为对应级别的模板函数
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

