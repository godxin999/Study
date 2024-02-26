module pluginable;

import plugin;
import stl;

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