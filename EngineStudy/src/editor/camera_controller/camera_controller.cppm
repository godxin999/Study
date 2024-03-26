export module camera_controller;

import glm;
import camera;
import input_manager;
import window_manager;
import stl;
import transform;

namespace Engine::inline editor{
    export class CameraController{
    public:
        CameraController(Camera& camera,bool enableFocusInput=false);
        ~CameraController()=default;
        void HandleInput(float deltaTime);
        void SetSpeed(float speed);
        float GetSpeed()const;
    private:
        void HandleCameraPan(glm::vec2 offset,bool firstMouse);
        void HandleCameraOrbit(glm::vec2 offset,bool firstMouse);
        void HandleCameraFPSMouse(glm::vec2 offset,bool firstMouse);
        void HandleCameraZoom();
        void HandleCameraFPSKeyboard(float deltaTime);
        void UpdateMouseState();

    private:
        Camera& m_camera;
        InputManager& m_inputManager;
        WindowManager& m_windowManager;

        bool m_enableFocusInput{false};
        bool m_firstMouse{true};
        bool m_leftMouseButtonPressed{false};
        bool m_rightMouseButtonPressed{false};
        bool m_middleMouseButtonPressed{false};
        std::pair<double, double> m_lastMousePos{0.0, 0.0};
        glm::vec3 m_eulerAngle{0.f};
    
        glm::vec3 m_targetSpeed{0.f};
        glm::vec3 m_currentSpeed{0.f};

        Transform* m_orbitTransform{nullptr};
        glm::vec3 m_orbitStartOffset{0.f};

        float m_mouseSensitivity{0.1f};
        float m_cameraDragSpeed{0.1f};
        float m_cameraOrbitSpeed{0.1f};
        float m_cameraMoveSpeed{0.1f};
        float m_focusDistance{10.f};
        float m_focusLerpCoefficient{0.8f};
    };
}