#pragma once
#include <format>
#include <source_location>
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <chrono>

namespace MyLog{
    #define MYLOG_FOREACH_LEVEL(f)\
        f(trace) \
        f(debug) \
        f(info) \
        f(critical) \
        f(warning) \
        f(error) \
        f(fatal)

    enum class Level : std::uint8_t{
        #define _FUNCTION(name) name,
            MYLOG_FOREACH_LEVEL(_FUNCTION)
        #undef _FUNCTION
    };

    namespace Details{
        inline std::string level_name(Level level){
            switch(level){
                #define _FUNCTION(name) case Level::name: return #name;
                    MYLOG_FOREACH_LEVEL(_FUNCTION)
                #undef _FUNCTION
            }
            return "Unknown";
        }

        inline Level get_level_from_name(std::string level){
            #define _FUNCTION(name) if(level==#name) return Level::name;
                MYLOG_FOREACH_LEVEL(_FUNCTION)
            #undef _FUNCTION
            return Level::info;
        } 

        template <class T>
        struct with_source_location{
            T inner;
            std::source_location loc;
            public:
            template <class U> requires std::is_constructible_v<T,U>
            consteval with_source_location(U &&inner,std::source_location loc  = std::source_location::current()):inner(std::forward<U>(inner)),loc (std::move(loc)){}
            constexpr const T& format()const{
                return inner;
            }
            constexpr const std::source_location& location()const{
                return loc;
            }
        };
        //$env:MYLOG_LEVEL="debug"
        inline Level max_level = []()->Level{
            if(auto name=std::getenv("MYLOG_LEVEL")){
                return get_level_from_name(name);
            }
            return Level::info;
        }();
        //$env:MYLOG_PATH="test.log"
        inline std::ofstream log_path = []()->std::ofstream{
            if(auto path=getenv("MYLOG_PATH")){
                return std::ofstream(path,std::ios::app);
            }
            return std::ofstream("a.log",std::ios::app);
        }();

        inline void output_log(Level level,std::string msg,const std::source_location& loc){
            std::chrono::zoned_time now{std::chrono::current_zone(),std::chrono::system_clock::now()};
            msg=std::format("{} {}:{} [{}] {}",now,loc.file_name(),loc.line(),Details::level_name(level),msg);
            if(log_path){
                log_path<<msg<<std::endl;
            }
            if(level>=max_level){
                std::cout<<msg<<std::endl;
            }
        }
        
        
        template <typename... Args>
        void MyLog(Level level,with_source_location<std::format_string<Args...>> fmt,Args&&... args){
            const auto& loc=fmt.location();
            auto msg=std::vformat(fmt.format().get(),std::make_format_args(args...));
            output_log(level,std::move(msg),loc);
        }
    }

    inline void set_level(Level level){
        Details::max_level = level;
    }

    inline void set_log_path(std::string path){
        Details::log_path=std::ofstream(path,std::ios::app);
    }

    #define _FUNCTION(name) \
        template <typename... Args> \
        void MyLog_##name(Details::with_source_location<std::format_string<Args...>> fmt,Args&&... args){ \
            return Details::MyLog(Level::name,std::move(fmt),std::forward<Args>(args)...); \
        }
        MYLOG_FOREACH_LEVEL(_FUNCTION)
    #undef _FUNCTION

    #define MYLOG_P(x) ::MyLog::MyLog_debug(#x " = {}",x)

};

