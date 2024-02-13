#include "application.h"

import <stdexcept>;
import <iostream>;
import global_context;

int main(){
    RuntimeGlobalContext::StartSystem();
    try{
        auto app=new Application;
        app->Init();
        app->Run();
        app->Destroy();
        delete app;
    }
    catch(const std::exception& e){
        std::cerr<<e.what()<<std::endl;
        return 1;
    }
    RuntimeGlobalContext::ShutdowmSystem();
    return 0;
}