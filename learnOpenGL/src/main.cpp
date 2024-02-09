#include "application.h"

import <stdexcept>;
import <iostream>;

int main(){
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
    return 0;
}