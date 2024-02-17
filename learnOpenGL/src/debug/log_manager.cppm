export module log_manager;

import spdlog;
import stl;

namespace Engine::inline Debug{
    export enum class LogLevel:std::uint8_t{
        debug,
        info,
        warn,
        error,
        fatal
    };
    
    export class LogManager final{
    public:
        LogManager();
        ~LogManager();
        void Log(LogLevel level,std::string&& message);
        void SetLevel(LogLevel level);
    private:
        std::shared_ptr<spdlog::logger> m_Logger{nullptr};
    };
    
}

