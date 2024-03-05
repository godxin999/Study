export module event;

import stl;

namespace Engine::inline Tools{
    export using ListenerID = uint64_t;
    export template<typename... Args>
    class Event{
    public:
        using Callback=std::function<void(Args...)>;
        ListenerID AddListener(Callback callback);
        bool RemoveListener(ListenerID id);
        void RemoveAllListeners();
        uint64_t GetListenerCount() const;
        void Invoke(Args... args);
        ListenerID operator+=(Callback callback);
        bool operator-=(ListenerID id);
    private:
        std::unordered_map<ListenerID,Callback> m_listeners{};
        ListenerID m_availableID=0;
    };
}

module : private;

namespace Engine::inline Tools{
    template<typename... Args>
    ListenerID Event<Args...>::AddListener(Callback callback){
        auto listenerID=m_availableID++;
        m_listeners.emplace(listenerID,callback);
        return listenerID;
    }

    template<typename... Args>
    bool Event<Args...>::RemoveListener(ListenerID id){
        return m_listeners.erase(id);
    }

    template<typename... Args>
    void Event<Args...>::RemoveAllListeners(){
        m_listeners.clear();
    }

    template<typename... Args>
    uint64_t Event<Args...>::GetListenerCount() const{
        return m_listeners.size();
    }

    template<typename... Args>
    void Event<Args...>::Invoke(Args... args){
        for(const auto& callback:m_listeners|std::views::values){
            callback(args...);
        }
    }

    template<typename... Args>
    ListenerID Event<Args...>::operator+=(Callback callback){
        return AddListener(callback);
    }

    template<typename... Args>
    bool Event<Args...>::operator-=(ListenerID id){
        return RemoveListener(id);
    }
}