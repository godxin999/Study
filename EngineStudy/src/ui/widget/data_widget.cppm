export module data_widget;

import stl;
import widget;
import plugin;
import pluginable;
import data_dispatcher;

namespace Engine::inline UI{
    export template <typename T>
    class DataWidget : public Widget{
    public:
        DataWidget(T& data);
        void Draw()override;
        void NotifyChange();
    protected:
        T& data;
    };
}

module : private;

namespace Engine::inline UI{
    template <typename T>
    DataWidget<T>::DataWidget(T& data):data(data){
    
    }
 
    template <typename T>
    void DataWidget<T>::Draw(){
        if(enable){
            if(auto plugin=GetPlugin<DataDispatcher<T>>()){
                auto value=plugin->Gather();
                Widget::Draw();
                plugin->Provide(value);
            }
            else{
                Widget::Draw();
            }
        }
    }

    template <typename T>
    void DataWidget<T>::NotifyChange(){
        if(auto plugin=GetPlugin<DataDispatcher<T>>()){
            plugin->NotifyChange();
        }
    }
}