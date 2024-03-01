export module data_dispatcher;

import stl;
import plugin;


namespace Engine::inline UI{
    export template <typename T>
    class DataDispatcher:public Plugin{
    public:
        DataDispatcher()=default;
        virtual ~DataDispatcher()=default;
        void BindData(const T& data);
        void RegisterProvider(std::function<void(T)> provider);
        void RegisterGatherer(std::function<T()> gatherer);
        void NotifyChange();
        void Provide(T data);
        T Gather();
        void Execute() override;
    private:
        bool m_IsValueChanged{false};
        T* m_Data{nullptr};
        std::function<void(T)> m_Provider;
        std::function<T()> m_Gatherer;
    };
}

module : private;

namespace Engine::inline UI{
    template <typename T>
    void DataDispatcher<T>::BindData(const T& data){
        m_Data=std::addressof(data);
    }
    template <typename T>
    void DataDispatcher<T>::RegisterProvider(std::function<void(T)> provider){
        m_Provider=provider;
    }
    template <typename T>
    void DataDispatcher<T>::RegisterGatherer(std::function<T()> gatherer){
        m_Gatherer=gatherer;
    }
    template <typename T>
    void DataDispatcher<T>::NotifyChange(){
        m_IsValueChanged=true;
    }
    template <typename T>
    void DataDispatcher<T>::Provide(T data){
        if(m_IsValueChanged){
            if(m_Data){
                *m_Data=data;
            }
            else{
                m_Provider(data);
            }
            m_IsValueChanged=false;
        }
    }
    template <typename T>
    T DataDispatcher<T>::Gather(){
        if(m_Data){
            return *m_Data;
        }
        else{
            return m_Gatherer();
        }
    }
    template <typename T>
    void DataDispatcher<T>::Execute(){

    }
}