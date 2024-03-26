module scene_manager;

import stl;
import scene;
import event;

namespace Engine::inline Core{
    SceneManager::SceneManager(const std::u8string& sceneRootPath):m_sceneRootPath(sceneRootPath){
        LoadEmptyScene();
    }
    SceneManager::~SceneManager(){
        UnloadCurrentScene();
    }
    void SceneManager::Update(){
        if(m_delayLoadFunction){
            m_delayLoadFunction();
            m_delayLoadFunction = nullptr;
        }
    }
    void SceneManager::LoadEmptyScene(){
        UnloadCurrentScene();
        m_currentScene = new Scene();
        SceneLoadEvent();
    }
    void SceneManager::LoadEmptyLightedScene(){
        UnloadCurrentScene();
        m_currentScene = new Scene();
        SceneLoadEvent();

        //Light...
    }
    bool SceneManager::LoadScene(const std::u8string& path,bool absolutePath){
        
        return false;
    }
    bool SceneManager::LoadSceneFromFile(){
        
        return false;
    }
    void SceneManager::LoadAndPlayDelayed(const std::u8string& path,bool absolutePath){
        m_delayLoadFunction = [this,path,absolutePath](){
            std::u8string scenePath = GetCurrentScenePath();
            if(LoadScene(path,absolutePath)){
                GetCurrentScene()->Play();
            }
            else{
                StoreCurrentScenePath(scenePath);
            }
        };
    }
    void SceneManager::UnloadCurrentScene(){
        if(m_currentScene){
            delete m_currentScene;
            m_currentScene = nullptr;
            SceneUnloadEvent();
        }
        ClearCurrentScenePath();
    }
    Scene* SceneManager::GetCurrentScene(){
        return m_currentScene;
    }
    [[nodiscard]]bool SceneManager::HasCurrentScene()const{
        return m_currentScene;
    }
    [[nodiscard]]const std::u8string& SceneManager::GetCurrentScenePath()const{
        return m_currentScenePath;
    }
    [[nodiscard]]bool SceneManager::IsCurrentSceneLoadedFromFile()const{
        return m_currentSceneLoadedFromFile;
    }
    void SceneManager::StoreCurrentScenePath(const std::u8string& path){
        m_currentScenePath = path;
        m_currentSceneLoadedFromFile = true;
        CurrentScenePathChangeEvent(m_currentScenePath);
    }
    void SceneManager::ClearCurrentScenePath(){
        m_currentScenePath.clear();
        m_currentSceneLoadedFromFile = false;
        CurrentScenePathChangeEvent(m_currentScenePath);
    }
}