module;
#include <GLFW/glfw3.h>
export module device;
import input_manager.input_state;
import event;
import stl;


namespace Engine::inline Window{
    export enum class DeviceError{
        NOT_INITIALIZED		= 0x00010001,
		NO_CURRENT_CONTEXT	= 0x00010002,
		INVALID_ENUM		= 0x00010003,
		INVALID_VALUE		= 0x00010004,
		OUT_OF_MEMORY		= 0x00010005,
		API_UNAVAILABLE		= 0x00010006,
		VERSION_UNAVAILABLE = 0x00010007,
		PLATFORM_ERROR		= 0x00010008,
		FORMAT_UNAVAILABLE	= 0x00010009,
		NO_WINDOW_CONTEXT	= 0x0001000A
    };

    export class Device{
    public:
        Device();
        ~Device();
        std::pair<uint16_t,uint16_t> GetMonitorSize()const;
        GLFWcursor* GetCursorInstance(CursorShape shape)const;
        bool HasVsync()const;
        void SetVsync(bool vsync);
        void PollEvents()const;
        float GetElapsedTime()const;
    public:
        inline static Event<DeviceError,std::string> ErrorEvent{};
    private:
        void BindErrorCallback()const;
        void CreateCursors();
        void DestroyCursors();
        bool m_Vsync{true};
        bool m_IsInitialized{false};
        std::unordered_map<CursorShape,GLFWcursor*> m_Cursors;
    };
};