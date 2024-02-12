module;
#include <spdlog/pattern_formatter.h>
module global_context.log_system;

import spdlog;
import <memory>;
import <initializer_list>;

std::shared_ptr<LogSystem> RG_LogSystem{nullptr};

LogSystem::LogSystem(){
    spdlog::init_thread_pool(8192,1);

    auto console_sink=std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    auto file_sink=std::make_shared<spdlog::sinks::rotating_file_sink_mt>("log.txt",1024*1024*5,3);

    const std::initializer_list<spdlog::sink_ptr> sinks{console_sink,file_sink};

    m_logger=std::make_shared<spdlog::async_logger>("logger",sinks.begin(),sinks.end(),spdlog::thread_pool(),spdlog::async_overflow_policy::block);

    spdlog::register_logger(m_logger);
    
    m_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [thread %t] %v");

    m_logger->set_level(spdlog::level::trace);

    m_logger->info("Logger initialized");
}

LogSystem::~LogSystem(){
    m_logger->flush();
    spdlog::drop_all();   
}

void LogSystem::log(log_level level,std::string&& message){
    switch(level){
        case log_level::debug:
            m_logger->debug(message);
            break;
        case log_level::info:
            m_logger->info(message);
            break;
        case log_level::warn:
            m_logger->warn(message);
            break;
        case log_level::error:
            m_logger->error(message);
            break;
        case log_level::fatal:
            m_logger->critical(message);
            break;
        default:
            break;
    }

}
void LogSystem::set_level(log_level level){
    switch(level){
        case log_level::debug:
            m_logger->set_level(spdlog::level::debug);
            break;
        case log_level::info:
            m_logger->set_level(spdlog::level::info);
            break;
        case log_level::warn:
            m_logger->set_level(spdlog::level::warn);
            break;
        case log_level::error:
            m_logger->set_level(spdlog::level::err);
            break;
        case log_level::fatal:
            m_logger->set_level(spdlog::level::critical);
            break;
        default:
            break;
    }
}