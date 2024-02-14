export module context;
import stl;
import window_manager;
import log_manager;


namespace Engine::inline Editor{
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
        std::unique_ptr<LogManager> m_LogManager{nullptr};
    };
};