module;
#define SPDLOG_HEADER_ONLY

#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/details/registry.h>
#include <spdlog/fmt/fmt.h>

export module spdlog;
/*
namespace fmt {
    export using fmt::format;
    //export using fmt::print;
}

namespace spdlog {
    export using spdlog::logger;
    export using spdlog::sink_ptr;
    export using spdlog::drop_all;
    export using spdlog::register_logger;
    export using spdlog::async_logger;
    export using spdlog::async_overflow_policy;
    export using spdlog::init_thread_pool;
    export using spdlog::thread_pool;


    namespace sinks {
        export using spdlog::sinks::stdout_color_sink_mt;
        export using spdlog::sinks::rotating_file_sink_mt;
    }
    
    namespace level {
        export using spdlog::level::level_enum;
    }

    namespace details {
        export using spdlog::details::registry;
    }
}
*/


export namespace fmt {
    using fmt::format;
    //using fmt::print;
}

export namespace spdlog {
    using spdlog::logger;
    using spdlog::sink_ptr;
    using spdlog::drop_all;
    using spdlog::register_logger;
    using spdlog::async_logger;
    using spdlog::async_overflow_policy;
    using spdlog::init_thread_pool;
    using spdlog::thread_pool;


    namespace sinks {
        using spdlog::sinks::stdout_color_sink_mt;
        using spdlog::sinks::rotating_file_sink_mt;
    }
    
    namespace level {
        using spdlog::level::level_enum;
    }

    /*namespace details {
        export using spdlog::details::registry;
    }*/
}