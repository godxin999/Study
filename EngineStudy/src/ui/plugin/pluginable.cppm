export module pluginable;

import plugin;
import stl;

namespace Engine::inline UI{
    export class Pluginable{
    public:
        Pluginable()=default;
        virtual ~Pluginable();
        template <typename T,typename... Args>
        requires std::derived_from<T,Plugin>
        T& AddPlugin(Args&&... args){
            T* plugin = new T(std::forward<Args>(args)...);
            m_plugins.push_back(plugin);
            return *plugin;
        }
        template <typename T>
        requires std::derived_from<T,Plugin>
        T* GetPlugin(){
            for(auto& plugin : m_plugins){
                if(T* p=dynamic_cast<T*>(plugin)){
                    return p;
                }
            }
            return nullptr;
        }
        void ExecutePlugins();
        void RemoveAllPlugins();
    private:
        std::vector<Plugin*> m_plugins{};
    };
}

module : private;

namespace Engine::inline UI{
    void Pluginable::ExecutePlugins(){
        for(auto& plugin : m_plugins){
            plugin->Execute();
        }
    }

    void Pluginable::RemoveAllPlugins(){
        for(auto& plugin : m_plugins){
            delete plugin;
        }
        m_plugins.clear();
    }
    
    Pluginable::~Pluginable(){
        RemoveAllPlugins();
    }
}