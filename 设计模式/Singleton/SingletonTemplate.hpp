#include <memory>
#include <mutex>
#include <thread>
#include <iostream>

template <typename T>
class Singleton{
protected:
    Singleton()=default;
    Singleton(const Singleton&)=delete;
    Singleton& operator=(const Singleton&)=delete;
    static std::shared_ptr<T> _instance;
    static std::once_flag s_flag;
public:
    static std::shared_ptr<T> getInstance(){
        std::call_once(s_flag,[&](){
            _instance=std::shared_ptr<T>(new T);
        });
        return _instance;
    }
    void printAddress(){
        std::cout<<"Singleton address:"<<_instance.get()<<std::endl;
    }
    template <typename... Args>
    static std::shared_ptr<T> Construct(Args&&... args){
        std::call_once(s_flag,[&](){
            _instance=std::shared_ptr<T>(new T(std::forward<Args>(args)...));
        });
        return _instance;
    }

    virtual ~Singleton()=default;
};

template <typename T>
std::shared_ptr<T> Singleton<T>::_instance=nullptr;

template <typename T>
std::once_flag Singleton<T>::s_flag;
