module;
#include <GLFW/glfw3.h>
#include <glad/glad.h>
export module application;
import service_locator;
import stl;
import context;
import editor;
import window_manager;
import glm;
import camera;
import shader;
import buffer;
import texture;
import light;
import clock;
import transform;

float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
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


std::unique_ptr<Engine::VertexBuffer<float>> VBO;
std::unique_ptr<Engine::VertexArray> CubeVAO,LightCubeVAO;
Shader LightingShader,LightCubeShader;
Engine::Texture diffuseMap,specularMap,emissionMap;
Engine::Camera* camera{nullptr};
Engine::Light* light{nullptr};
Engine::Transform transform(glm::vec3(0.f,0.f,2.0f),glm::quat(),glm::vec3(1.f));

void render(){
    auto& context=Engine::ServiceLocator::Get<Engine::Context>();
    context.ubo->SetSubData(camera->GetViewMatrix(),0);
    context.ubo->SetSubData(camera->GetProjMatrix(),sizeof(glm::mat4));
    context.ubo->SetSubData(camera->GetPosition(),sizeof(glm::mat4)*2);
    context.ubo->Bind();
    LightingShader.Bind();
    context.lightSSBO->Bind(0);
    glm::mat4 model=glm::mat4(1.f);
    diffuseMap.Bind(0);
    specularMap.Bind(1);
    CubeVAO->Bind();
    for(int i=0;i<10;++i){
        model=glm::mat4(1.f);
        model=glm::translate(model,cubePositions[i]);
        float angle=20.f*i;
        model=glm::rotate(model,glm::radians(angle),glm::vec3(1.f,.3f,.5f));
        LightingShader.SetMat4("model",model);
        LightingShader.SetMat4("transInvModel",glm::transpose(glm::inverse(model)));
        glDrawArrays(GL_TRIANGLES,0,36);
    }
    /*LightCubeShader.Bind();
    
    model=glm::mat4(1.f);
    model=glm::translate(model,lightPos);
    model=glm::scale(model,glm::vec3(.2f));
    
    LightCubeShader.SetMat4("model",model);
    LightCubeShader.SetMat4("view",camera->GetViewMatrix());
    LightCubeShader.SetMat4("projection",camera->GetProjMatrix());

    LightCubeVAO->Bind();
    glDrawArrays(GL_TRIANGLES,0,36);*/
}
void update(){
    camera->Update();
}
void init(){
    //启用深度测试
    glEnable(GL_DEPTH_TEST);

    LightingShader=Shader("../../../../shaders/light.vs","../../../../shaders/light.fs");
    LightCubeShader=Shader("../../../../shaders/light_cube.vs","../../../../shaders/light_cube.fs");

    VBO=std::make_unique<Engine::VertexBuffer<float>>(vertices,sizeof(vertices)/sizeof(float));
    CubeVAO=std::make_unique<Engine::VertexArray>();
    CubeVAO->Bind();
    VBO->Bind();
    CubeVAO->BindAttribute(0,3,Engine::Render::DataType::FLOAT,false,8*sizeof(float),(void*)0);
    CubeVAO->BindAttribute(1,3,Engine::Render::DataType::FLOAT,false,8*sizeof(float),(void*)(3*sizeof(float)));
    CubeVAO->BindAttribute(2,2,Engine::Render::DataType::FLOAT,false,8*sizeof(float),(void*)(6*sizeof(float)));
    CubeVAO->Unbind();
    LightCubeVAO=std::make_unique<Engine::VertexArray>();
    LightCubeVAO->Bind();
    LightCubeVAO->BindAttribute(0,3,Engine::Render::DataType::FLOAT,false,8*sizeof(float),(void*)0);

    diffuseMap.LoadTexture("../../../../assets/container2.png");
    specularMap.LoadTexture("../../../../assets/container2_specular.png");
    emissionMap.LoadTexture("../../../../assets/matrix.jpg");
    LightingShader.Bind();
    LightingShader.SetInt("u_DiffuseMap",0);
    LightingShader.SetInt("u_SpecularMap",1);
    //LightingShader.SetInt("u_Emission",2);

    Engine::UniformBuffer::BindBlockToShader(LightingShader,Engine::UniformBuffer::GetBlockLocation(LightingShader,"UBO"),0);

    LightingShader.Unbind();
    
    VBO->Unbind();
    LightCubeVAO->Unbind();

    camera=new Engine::Camera(45.f,800.f/600.f,.1f,1000.f);
    camera->SetPostion(glm::vec3(0.f,0.f,10.f));

}

namespace Engine::inline editor{
    export class Application{
    public:
        Application():m_context(),m_editor(m_context){

        }
        ~Application()=default;
        void Run(){
            /*init();
            while(IsRunning()){
                update();

                glClearColor(.1f,.1f,.1f,1.f);
                glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//清除颜色缓冲和深度缓冲
                
                render();

                glfwSwapBuffers(m_context.m_windowManager->GetGlfwWindow());//交换颜色缓冲
                glfwPollEvents();//检查是否触发事件并调用对应的回调函数
            }*/
            Clock clock;
            while(IsRunning()){
                m_editor.PreUpdate();
                m_editor.Update(clock.GetDeltaTime());
                m_editor.PostUpdate();
                clock.Update();
            }
        }
        bool IsRunning()const{
            return !m_context.windowManager->IsShouldClose();
        }
    private:
        Context m_context;
        Editor m_editor;
    };
};