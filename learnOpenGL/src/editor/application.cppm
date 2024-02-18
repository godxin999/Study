module;
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stb/stb_image.h>
export module application;
import stl;
import context;
import window_manager;
import glm;
import camera;
import shader;
import buffer;

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
unsigned diffuseMap,specularMap,emissionMap;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
Engine::Camera* camera{nullptr};

void render(){
    LightingShader.Use();

    LightingShader.SetFloat("matrixLight",(1.f+sin(glfwGetTime()))/2.f+.5f);
    LightingShader.SetFloat("matrixMove",glfwGetTime());
    LightingShader.SetVec3("light.position",lightPos);
    LightingShader.SetVec3("viewPos",camera->GetPosition());
    LightingShader.SetVec3("light.specular",1.f,1.f,1.f);
    LightingShader.SetVec3("light.ambient",.2f,.2f,.2f);
    LightingShader.SetVec3("light.diffuse",.5f,.5f,.5f);
    LightingShader.SetFloat("material.shininess",64.f);

    glm::mat4 model=glm::mat4(1.f);
    LightingShader.SetMat4("model",model);
    LightingShader.SetMat4("view",camera->GetViewMatrix());
    LightingShader.SetMat4("projection",camera->GetProjMatrix());
    LightingShader.SetMat4("transInvModel",glm::transpose(glm::inverse(model)));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,specularMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,emissionMap);

    CubeVAO->Bind();
    glDrawArrays(GL_TRIANGLES,0,36);

    LightCubeShader.Use();
    
    model=glm::mat4(1.f);
    model=glm::translate(model,lightPos);
    model=glm::scale(model,glm::vec3(.2f));
    
    LightCubeShader.SetMat4("model",model);
    LightCubeShader.SetMat4("view",camera->GetViewMatrix());
    LightCubeShader.SetMat4("projection",camera->GetProjMatrix());

    LightCubeVAO->Bind();
    glDrawArrays(GL_TRIANGLES,0,36);
}
void update(){
    camera->Update();
}
void init(){
    //启用深度测试
    glEnable(GL_DEPTH_TEST);

    LightingShader=Shader("../../../../shaders/lighting_map.vs","../../../../shaders/lighting_map.fs");
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

    glGenTextures(1,&diffuseMap);
    int width,height,nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data=stbi_load("../../../../assets/container2.png",&width,&height,&nrChannels,0);
    if(data){
        GLenum format;
        if(nrChannels==1){
            format=GL_RED;
        }else if(nrChannels==3){
            format=GL_RGB;
        }else if(nrChannels==4){
            format=GL_RGBA;
        }
        glBindTexture(GL_TEXTURE_2D,diffuseMap);
        glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,format,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    }
    stbi_image_free(data);

    glGenTextures(1,&specularMap);
    //data=stbi_load("../../../../assets/container2_specular.png",&width,&height,&nrChannels,0);
    data=stbi_load("../../../../assets/lighting_maps_specular_color.png",&width,&height,&nrChannels,0);
    if(data){
        GLenum format;
        if(nrChannels==1){
            format=GL_RED;
        }else if(nrChannels==3){
            format=GL_RGB;
        }else if(nrChannels==4){
            format=GL_RGBA;
        }
        glBindTexture(GL_TEXTURE_2D,specularMap);
        glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,format,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    }
    stbi_image_free(data);

    glGenTextures(1,&emissionMap);
    data=stbi_load("../../../../assets/matrix.jpg",&width,&height,&nrChannels,0);
    if(data){
        GLenum format;
        if(nrChannels==1){
            format=GL_RED;
        }else if(nrChannels==3){
            format=GL_RGB;
        }else if(nrChannels==4){
            format=GL_RGBA;
        }
        glBindTexture(GL_TEXTURE_2D,emissionMap);
        glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,format,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    }
    stbi_image_free(data);

    LightingShader.Use();
    LightingShader.SetInt("material.diffuse",0);
    LightingShader.SetInt("material.specular",1);
    LightingShader.SetInt("material.emission",2);

    VBO->Unbind();
    LightCubeVAO->Unbind();

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