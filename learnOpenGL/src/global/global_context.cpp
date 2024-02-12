module global_context;

import <memory>;
//import global_context.log_system;


void RuntimeGlobalContext::StartSystem(){
    RG_LogSystem = std::make_shared<LogSystem>();
    RG_LogSystem->set_level(log_level::debug);
    
    
    RG_LogSystem->log(log_level::info,"RuntimeGlobalContext::StartSystem");
}
void RuntimeGlobalContext::ShutdowmSystem(){
    RG_LogSystem->log(log_level::info,"RuntimeGlobalContext::ShutdowmSystem");
    RG_LogSystem.reset();
}