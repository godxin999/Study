export module scene_manager;

import stl;
import scene;
import event;

namespace Engine::inline Core{
    export class SceneManager{
    public:
        SceneManager(const std::u8string& sceneRootPath);
        ~SceneManager();
        void Update();
        void LoadEmptyScene();
        void LoadEmptyLightedScene();
        bool LoadScene(const std::u8string& path,bool absolutePath=false);
        bool LoadSceneFromFile();
        void LoadAndPlayDelayed(const std::u8string& path,bool absolutePath=false);
        void UnloadCurrentScene();
        Scene* GetCurrentScene();
        [[nodiscard]]bool HasCurrentScene()const;
        [[nodiscard]]const std::u8string& GetCurrentScenePath()const;
        [[nodiscard]]bool IsCurrentSceneLoadedFromFile()const;
        void StoreCurrentScenePath(const std::u8string& path);
        void ClearCurrentScenePath();
    public:
        Event<> SceneLoadEvent{};
        Event<> SceneUnloadEvent{};
        Event<const std::u8string&> CurrentScenePathChangeEvent{};
    private:
        const std::u8string m_sceneRootPath;//场景根目录
        Scene* m_currentScene{nullptr};
        bool m_currentSceneLoadedFromFile{false};
        std::u8string m_currentScenePath{};
        std::function<void()> m_delayLoadFunction{nullptr};
    };
}