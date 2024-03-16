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
        bool m_isValueChanged{false};
        T* m_data{nullptr};
        std::function<void(T)> m_provider;
        std::function<T()> m_gatherer;
    };
}

module : private;

namespace Engine::inline UI{
    template <typename T>
    void DataDispatcher<T>::BindData(const T& data){
        m_data=std::addressof(data);
    }

    template <typename T>
    void DataDispatcher<T>::RegisterProvider(std::function<void(T)> provider){
        m_provider=provider;
    }

    template <typename T>
    void DataDispatcher<T>::RegisterGatherer(std::function<T()> gatherer){
        m_gatherer=gatherer;
    }

    template <typename T>
    void DataDispatcher<T>::NotifyChange(){
        m_isValueChanged=true;
    }

    template <typename T>
    void DataDispatcher<T>::Provide(T data){
        if(m_isValueChanged){
            if(m_data){
                *m_data=data;
            }
            else{
                m_provider(data);
            }
            m_isValueChanged=false;
        }
    }

    template <typename T>
    T DataDispatcher<T>::Gather(){
        if(m_data){
            return *m_data;
        }
        else{
            return m_gatherer();
        }
    }
    
    template <typename T>
    void DataDispatcher<T>::Execute(){

    }
}