module;
#include <GLFW/glfw3.h>
#include <glad/glad.h>
export module application;
import stl;
import context;
import window_manager;
import glm;
import camera;
import shader;


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
Engine::Camera* camera{nullptr};

void render(){
    LightingShader.Use();

    LightingShader.SetVec3("objectColor",1.f,.5f,.31f);
    LightingShader.SetVec3("lightColor",1.f,1.f,1.f);
    LightingShader.SetVec3("lightPos",lightPos);
    LightingShader.SetVec3("viewPos",camera->GetPosition());

    glm::mat4 model=glm::mat4(1.f);
    LightingShader.SetMat4("model",model);
    LightingShader.SetMat4("view",camera->GetViewMatrix());
    LightingShader.SetMat4("projection",camera->GetProjMatrix());
    LightingShader.SetMat4("transInvModel",glm::transpose(glm::inverse(model)));

    glBindVertexArray(CubeVAO);
    glDrawArrays(GL_TRIANGLES,0,36);

    LightCubeShader.Use();
    
    model=glm::mat4(1.f);
    model=glm::translate(model,lightPos);
    model=glm::scale(model,glm::vec3(.2f));
    
    LightCubeShader.SetMat4("model",model);
    LightCubeShader.SetMat4("view",camera->GetViewMatrix());
    LightCubeShader.SetMat4("projection",camera->GetProjMatrix());

    glBindVertexArray(LightCubeVAO);
    glDrawArrays(GL_TRIANGLES,0,36);
}
void update(){
    camera->Update();
}
void init(){
    
    

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

    camera=new Engine::Camera(45.f,800.f/600.f,.1f,1000.f);
    camera->SetPostion(glm::vec3(0.f,0.f,3.f));
}

namespace Engine::inline Editor{
    export class Application{
    public:
        Application(){

        }
        ~Application(){

        }
        void Run(){
            init();
            while(IsRunning()){
                update();

                glClearColor(.1f,.1f,.1f,1.f);
                glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//清除颜色缓冲和深度缓冲
                
                render();

                glfwSwapBuffers(m_Context.m_WindowManager->GetGlfwWindow());//交换颜色缓冲
                glfwPollEvents();//检查是否触发事件并调用对应的回调函数
            }
        }
        bool IsRunning()const{
            return !m_Context.m_WindowManager->IsShouldClose();
        }
    private:
        Context m_Context;
    };
};