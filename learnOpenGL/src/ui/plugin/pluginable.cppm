export module pluginable;

import plugin;
import stl;
import data_dispatcher;

namespace Engine::inline UI{
    export class Pluginable{
    public:
        Pluginable()=default;
        virtual ~Pluginable();
        template <typename T,typename... Args>
        requires std::derived_from<T,Plugin>
        T& AddPlugin(Args&&... args){
            T* plugin = new T(std::forward<Args>(args)...);
            m_Plugins.push_back(plugin);
            return *plugin;
        }
        template <typename T>
        requires std::derived_from<T,Plugin>
        T* GetPlugin(){
            for(auto& plugin : m_Plugins){
                if(T* p=dynamic_cast<T*>(plugin)){
                    return p;
                }
            }
            return nullptr;
        }
        void ExecutePlugins();
        void RemoveAllPlugins();
    private:
        std::vector<Plugin*> m_Plugins{};
    };
}

module : private;

namespace Engine::inline UI{
    void Pluginable::ExecutePlugins(){
        for(auto& plugin : m_Plugins){
            plugin->Execute();
        }
    }

    void Pluginable::RemoveAllPlugins(){
        for(auto& plugin : m_Plugins){
            delete plugin;
        }
        m_Plugins.clear();
    }
    
    Pluginable::~Pluginable(){
        RemoveAllPlugins();
    }
}