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
        const std::string m_ProjectPath;
        const std::string m_ProjectName;
        const std::string m_ProjectAssetsPath;

        std::unique_ptr<Device> m_Device{nullptr};
        std::unique_ptr<WindowManager> m_WindowManager{nullptr};
        std::unique_ptr<InputManager> m_InputManager{nullptr};
        std::unique_ptr<LogManager> m_LogManager{nullptr};
        std::unique_ptr<UIManager> m_UIManager{nullptr};

        std::unique_ptr<UniformBuffer> m_UBO{nullptr};
        std::unique_ptr<ShaderStorageBuffer> m_LightSSBO{nullptr};
    };
};