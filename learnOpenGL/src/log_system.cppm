export module log_system;

import spdlog;
import <memory>;
import <string>;

export enum class log_level:std::uint8_t{
    debug,
    info,
    warn,
    error,
    fatal
};

export class LogSystem final{
public:
    LogSystem();
    ~LogSystem();
    void log(log_level level,std::string&& message);
    void set_level(log_level level);
private:
    std::shared_ptr<spdlog::logger> m_logger{nullptr};
};
