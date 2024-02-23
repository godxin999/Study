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
import buffer;
import texture;
import light;
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

//unsigned CubeVAO,LightCubeVAO,VBO;
std::unique_ptr<Engine::VertexBuffer<float>> VBO;
std::unique_ptr<Engine::VertexArray> CubeVAO,LightCubeVAO;
Shader LightingShader,LightCubeShader;
Engine::Texture diffuseMap,specularMap,emissionMap;
Engine::Texture spotLightMap;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
Engine::Camera* camera{nullptr};
Engine::Light* light{nullptr};
Engine::Transform transform(glm::vec3(0.f,0.f,2.0f),glm::quat(),glm::vec3(1.f));

void render(){
    LightingShader.Bind();
    LightingShader.SetMat4("light",light->GenerateDataMatrix());
    LightingShader.SetFloat("matrixLight",(1.f+sin(glfwGetTime()))/2.f+.5f);
    LightingShader.SetFloat("matrixMove",glfwGetTime());
    LightingShader.SetVec3("viewPos",camera->GetPosition());    
    LightingShader.SetFloat("material.shininess",64.f);

    glm::mat4 model=glm::mat4(1.f);
    //LightingShader.SetMat4("model",model);
    LightingShader.SetMat4("view",camera->GetViewMatrix());
    LightingShader.SetMat4("projection",camera->GetProjMatrix());
    //LightingShader.SetMat4("transInvModel",glm::transpose(glm::inverse(model)));

    diffuseMap.Bind(0);
    specularMap.Bind(1);
    //emissionMap.Bind(2);
    spotLightMap.Bind(3);

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

    spotLightMap.LoadTexture("../../../../assets/awesomeface.png");
    LightingShader.Bind();
    LightingShader.SetInt("material.diffuse",0);
    LightingShader.SetInt("material.specular",1);
    LightingShader.SetInt("material.emission",2);
    LightingShader.SetInt("spotLightMap",3);
    LightingShader.Unbind();
    
    VBO->Unbind();
    LightCubeVAO->Unbind();

    camera=new Engine::Camera(45.f,800.f/600.f,.1f,1000.f);
    camera->SetPostion(glm::vec3(0.f,0.f,2.f));

    transform.SetWorldRotation(glm::angleAxis(glm::radians(180.f),glm::vec3(0.f,1.f,0.f)));
    //light=new Engine::Light(transform,Engine::LightType::Directional);
    //light=new Engine::Light(transform,Engine::LightType::Point);
    light=new Engine::Light(transform,Engine::LightType::Spot);
    light->m_AttCoeff=glm::vec3(1.f,0.09f,0.032f);
    //light->m_AttCoeff=glm::vec3(1.f,0.022f,0.0019f);
    light->m_Cutoff=10.f;
    light->m_OuterCutoff=15.f;
    glm::mat4 data=light->GenerateDataMatrix();
}

namespace Engine::inline Editor{
    export class Application{
    public:
        Application()=default;
        ~Application()=default;
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