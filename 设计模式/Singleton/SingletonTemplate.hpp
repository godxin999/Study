#include <memory>
#include <mutex>
#include <thread>
#include <iostream>
#include <format>

template <typename T>
class Singleton{
protected:
    Singleton()=default;
    Singleton(const Singleton&)=delete;
    Singleton& operator=(const Singleton&)=delete;
    inline static std::shared_ptr<T> _instance=nullptr;
    inline static std::once_flag s_flag;
public:
    void printAddress(){
        std::cout << std::format("Singleton address: {}\n", static_cast<void*>(_instance.get()));
    }
    static std::shared_ptr<T> getInstance(){
        std::call_once(s_flag,[&](){
            _instance=std::shared_ptr<T>(new T);
        });
        return _instance;
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