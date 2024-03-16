import application;
import stl;
int main(){
    try{
        Engine::Application app;
        app.Run();
    }
    catch(const std::exception& e){
        std::cerr<<e.what()<<std::endl;
    }
    return 0;
}