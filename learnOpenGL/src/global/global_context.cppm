module;

export module global_context;
import <memory>;
import log_system;

export class RuntimeGlobalContext{
public:
    void StartSystem();
    void ShutdowmSystem();
public:
    std::shared_ptr<LogSystem> m_LogSystem;
};

export extern RuntimeGlobalContext g_RuntimeGlobalContext;