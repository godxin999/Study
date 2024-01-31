#pragma once

class GLFWwindow;
class Application{
public:
    static void Init();
    static void Run();
    static void Exit();
    static void Destroy();
    static void Update();
    static void Render();
    static void ProcessInput();
private:
    inline static GLFWwindow* window{nullptr};
};