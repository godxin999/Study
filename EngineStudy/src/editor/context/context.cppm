export module context;
import stl;
import buffer;
import ui_manager;
import window_manager;
import input_manager;
import log_manager;
import device;
import renderer;
import scene_manager;


namespace Engine::inline editor{
    export class Context{
    public:
        Context();
        ~Context();
    public:
        const std::u8string projectPath;
        const std::u8string projectName;
        const std::u8string projectAssetsPath;

        std::unique_ptr<Device> device{nullptr};
        std::unique_ptr<WindowManager> windowManager{nullptr};
        std::unique_ptr<InputManager> inputManager{nullptr};
        std::unique_ptr<LogManager> logManager{nullptr};
        std::unique_ptr<Renderer> renderer{nullptr};
        std::unique_ptr<UIManager> uiManager{nullptr};

        std::unique_ptr<UniformBuffer> ubo{nullptr};
        std::unique_ptr<ShaderStorageBuffer> lightSSBO{nullptr};

        SceneManager sceneManager;
    };
};