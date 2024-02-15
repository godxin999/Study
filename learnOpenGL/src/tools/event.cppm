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
        [[nodiscard]]uint64_t GetListenerCount() const;
        void Invoke(Args&&... args);
        ListenerID operator+=(Callback callback);
        bool operator-=(ListenerID id);
    private:
        std::unordered_map<ListenerID,Callback> m_Listeners;
        ListenerID m_AvailableID=0;
    };
}

module : private;

namespace Engine::inline Tools{
    template<typename... Args>
    ListenerID Event<Args...>::AddListener(Callback callback){
        auto listenerID=m_AvailableID++;
        m_Listeners.emplace(listenerID,callback);
        return listenerID;
    }
    template<typename... Args>
    bool Event<Args...>::RemoveListener(ListenerID id){
        return m_Listeners.erase(id);
    }
    template<typename... Args>
    void Event<Args...>::RemoveAllListeners(){
        m_Listeners.clear();
    }
    template<typename... Args>
    uint64_t Event<Args...>::GetListenerCount() const{
        return m_Listeners.size();
    }
    template<typename... Args>
    void Event<Args...>::Invoke(Args&&... args){
        for(const auto& callback:m_Listeners|std::views::values){
            callback(std::forward<Args>(args)...);
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