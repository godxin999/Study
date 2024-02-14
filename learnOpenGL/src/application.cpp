#define STB_IMAGE_IMPLEMENTATION
#include "application.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
import <stdexcept>;
import <memory>;

import glm;
import camera;
import shader;
import screen;
import input_system;
import global_context;

float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

unsigned CubeVAO,LightCubeVAO,VBO;
Shader LightingShader,LightCubeShader;
unsigned texture1,texture2;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
Camera camera{45.f,800.f/600.f,.1f,1000.f};

//窗口大小改变时的回调函数
static void framebuffer_size_callback(GLFWwindow* window,int width,int height){
    glViewport(0,0,width,height);//前两个参数为窗口的左下角位置，后两个参数为渲染窗口的宽高
    Screen::SetWidthHeight(width,height);
}

static void error_callback(int error, const char* description) {
    RG_LogSystem->log(log_level::error,description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Input::RecordKey(key, action);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	Input::RecordKey(button, action);
}

static void mouse_move_callback(GLFWwindow* window, double x, double y) {
	if(x<0||y<0||x>Screen::GetWidth() || y>Screen::GetHeight())return;
    Input::SetMousePosition(x, y);
}

static void mouse_scroll_callback(GLFWwindow* window, double x, double y) {
	Input::RecordScroll(y);
}

void Application::Init(){
    RG_LogSystem->log(log_level::info,"Application is initializing...");

    glfwSetErrorCallback(error_callback);
    
    if(!glfwInit()){
        RG_LogSystem->log(log_level::error,"Failed to initialize GLFW");
        throw std::runtime_error("Failed to initialize GLFW");
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);//主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);//次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);//核心模式

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
#endif

    window =glfwCreateWindow(800,600,"LearnOpenGL",nullptr,nullptr);
    if(!window){
        glfwTerminate();
        RG_LogSystem->log(log_level::error,"Failed to create GLFW window");
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);//注册窗口大小改变的回调函数
    glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);
	glfwSetCursorPosCallback(window, mouse_move_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        RG_LogSystem->log(log_level::error,"Failed to initialize GLAD");
        throw std::runtime_error("Failed to initialize GLAD");
    }
    
    //glfwSwapInterval(1);

    //启用深度测试
    glEnable(GL_DEPTH_TEST);

    LightingShader=Shader("../../../../shaders/phong.vs","../../../../shaders/phong.fs");
    LightCubeShader=Shader("../../../../shaders/light_cube.vs","../../../../shaders/light_cube.fs");

    //创建VAO
    glGenVertexArrays(1,&CubeVAO);
    //创建VBO
    glGenBuffers(1,&VBO);
    //绑定VAO
    glBindVertexArray(CubeVAO);
    //绑定VBO
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    //将顶点数据复制到VBO中
    //GL_STATIC_DRAW表示数据不会或几乎不会改变
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glGenVertexArrays(1,&LightCubeVAO);
    glBindVertexArray(LightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    //解绑VBO
    glBindBuffer(GL_ARRAY_BUFFER,0);
    //解绑VAO
    glBindVertexArray(0);

    camera.SetPostion(glm::vec3(0.f,0.f,3.f));
}

void Application::Run(){
    while(!glfwWindowShouldClose(window)){
        ProcessInput();

        Update();

        glClearColor(.1f,.1f,.1f,1.f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//清除颜色缓冲和深度缓冲
        
        Render();

        glfwSwapBuffers(window);//交换颜色缓冲
        glfwPollEvents();//检查是否触发事件并调用对应的回调函数
    }
}

void Application::Exit(){
    glfwSetWindowShouldClose(window,true);
}

void Application::Destroy(){
    glDeleteVertexArrays(1,&CubeVAO);
    glDeleteVertexArrays(1,&LightCubeVAO);
    glDeleteBuffers(1,&VBO);
//    glDeleteBuffers(1,&EBO);
    glDeleteProgram(LightingShader.GetShaderProgramId());
    glfwTerminate();
}

void Application::Update(){
    camera.Update();
    Input::Update();
}

void Application::Render(){
    LightingShader.Use();

    LightingShader.SetVec3("objectColor",1.f,.5f,.31f);
    LightingShader.SetVec3("lightColor",1.f,1.f,1.f);
    LightingShader.SetVec3("lightPos",lightPos);
    LightingShader.SetVec3("viewPos",camera.GetPosition());

    glm::mat4 model=glm::mat4(1.f);
    LightingShader.SetMat4("model",model);
    LightingShader.SetMat4("view",camera.GetViewMatrix());
    LightingShader.SetMat4("projection",camera.GetProjMatrix());
    LightingShader.SetMat4("transInvModel",glm::transpose(glm::inverse(model)));

    glBindVertexArray(CubeVAO);
    glDrawArrays(GL_TRIANGLES,0,36);

    LightCubeShader.Use();
    
    model=glm::mat4(1.f);
    model=glm::translate(model,lightPos);
    model=glm::scale(model,glm::vec3(.2f));
    
    LightCubeShader.SetMat4("model",model);
    LightCubeShader.SetMat4("view",camera.GetViewMatrix());
    LightCubeShader.SetMat4("projection",camera.GetProjMatrix());

    glBindVertexArray(LightCubeVAO);
    glDrawArrays(GL_TRIANGLES,0,36);
}

void Application::ProcessInput(){
    //当用户按下esc键时，关闭应用程序
    if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS){
        glfwSetWindowShouldClose(window,true);
    }
    if(glfwGetKey(window,GLFW_KEY_F)==GLFW_PRESS){
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }
    if(glfwGetKey(window,GLFW_KEY_L)==GLFW_PRESS){
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }
}