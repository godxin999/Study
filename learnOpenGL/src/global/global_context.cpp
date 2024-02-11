module global_context;

import <memory>;
import log_system;

RuntimeGlobalContext g_RuntimeGlobalContext;

void RuntimeGlobalContext::StartSystem(){
    m_LogSystem = std::make_shared<LogSystem>();
    m_LogSystem->set_level(log_level::debug);
    
    
    m_LogSystem->log(log_level::info,"RuntimeGlobalContext::StartSystem");
}
void RuntimeGlobalContext::ShutdowmSystem(){
    m_LogSystem->log(log_level::info,"RuntimeGlobalContext::ShutdowmSystem");
    m_LogSystem.reset();
}