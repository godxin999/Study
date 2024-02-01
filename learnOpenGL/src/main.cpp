#include "application.h"

int main(){
    auto app=new Application;
    app->Init();
    app->Run();
    app->Destroy();
    delete app;
    return 0;
}