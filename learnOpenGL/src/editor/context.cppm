export module context;
import stl;
import buffer;
import ui_manager;
import window_manager;
import input_manager;
import log_manager;
import device;


namespace Engine::inline editor{
    export class Context{
    public:
        Context();
        ~Context();
    public:
        const std::string projectPath;
        const std::string projectName;
        const std::string projectAssetsPath;

        std::unique_ptr<Device> device{nullptr};
        std::unique_ptr<WindowManager> windowManager{nullptr};
        std::unique_ptr<InputManager> inputManager{nullptr};
        std::unique_ptr<LogManager> logManager{nullptr};
        std::unique_ptr<UIManager> uiManager{nullptr};

        std::unique_ptr<UniformBuffer> ubo{nullptr};
        std::unique_ptr<ShaderStorageBuffer> lightSSBO{nullptr};
    };
};