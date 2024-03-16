module;

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/details/registry.h>

export module spdlog;

export namespace spdlog {
    using spdlog::logger;
    using spdlog::sink_ptr;
    using spdlog::async_logger;
    using spdlog::async_overflow_policy;
    using spdlog::drop_all;
    using spdlog::register_logger;
    using spdlog::init_thread_pool;
    using spdlog::thread_pool;

    namespace sinks {
        using spdlog::sinks::stdout_color_sink_mt;
        using spdlog::sinks::rotating_file_sink_mt;
    }
    
    namespace level {
        using spdlog::level::level_enum;
    }

    namespace details {
        using spdlog::details::registry;
    }
    
}