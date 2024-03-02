module;
#include <spdlog/pattern_formatter.h>
module log_manager;

import spdlog;
import stl;

namespace Engine::inline Debug{
    LogManager::LogManager(){
        spdlog::init_thread_pool(8192,1);

        auto console_sink=std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink=std::make_shared<spdlog::sinks::rotating_file_sink_mt>("log.txt",1024*1024*5,3);

        const std::initializer_list<spdlog::sink_ptr> sinks {console_sink,file_sink};

        m_Logger=std::make_shared<spdlog::async_logger>("logger",sinks.begin(),sinks.end(),spdlog::thread_pool(),spdlog::async_overflow_policy::block);

        spdlog::register_logger(m_Logger);

        m_Logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [thread %t] %v");
        m_Logger->set_level(spdlog::level::trace);

        m_Logger->info("Logger initialized");
    }

    LogManager::~LogManager(){
        m_Logger->flush();
        spdlog::drop_all();   
    }

    void LogManager::Log(LogLevel level,std::string&& message){
        switch(level){
            case LogLevel::debug:
                m_Logger->debug(message);
                break;
            case LogLevel::info:
                m_Logger->info(message);
                break;
            case LogLevel::warn:
                m_Logger->warn(message);
                break;
            case LogLevel::error:
                m_Logger->error(message);
                break;
            case LogLevel::fatal:
                m_Logger->critical(message);
                break;
            default:
                break;
        }
    }

    void LogManager::SetLevel(LogLevel level){
        switch(level){
            case LogLevel::debug:
                m_Logger->set_level(spdlog::level::debug);
                break;
            case LogLevel::info:
                m_Logger->set_level(spdlog::level::info);
                break;
            case LogLevel::warn:
                m_Logger->set_level(spdlog::level::warn);
                break;
            case LogLevel::error:
                m_Logger->set_level(spdlog::level::err);
                break;
            case LogLevel::fatal:
                m_Logger->set_level(spdlog::level::critical);
                break;
            default:
                break;
        }
    }
}

