#include "spdlog/spdlog.h"
#include <memory>
#define STB_IMAGE_IMPLEMENTATION
#include "application.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
import <stdexcept>;

import glm;
import shader;
import log_system;

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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

unsigned VAO,VBO,EBO;
Shader shader;
unsigned texture1,texture2;

std::shared_ptr<LogSystem> logger=std::make_shared<LogSystem>();

//窗口大小改变时的回调函数
void framebuffer_size_callback(GLFWwindow* window,int width,int height){
    glViewport(0,0,width,height);//前两个参数为窗口的左下角位置，后两个参数为渲染窗口的宽高
}

void Application::Init(){

    logger->set_level(log_level::warn);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);//主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);//次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);//核心模式

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
#endif

    window =glfwCreateWindow(800,600,"LearnOpenGL",nullptr,nullptr);
    if(!window){
        glfwTerminate();
        logger->log(log_level::error,"Failed to create GLFW window");
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);//注册窗口大小改变的回调函数

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        logger->log(log_level::error,"Failed to initialize GLAD");
        throw std::runtime_error("Failed to initialize GLAD");
    }
    //启用深度测试
    glEnable(GL_DEPTH_TEST);

    shader=Shader("../../../../shaders/test.vs","../../../../shaders/test.fs");

    //创建VAO
    glGenVertexArrays(1,&VAO);
    //创建VBO
    glGenBuffers(1,&VBO);
    //创建EBO
    //glGenBuffers(1,&EBO);
    //绑定VAO
    glBindVertexArray(VAO);
    //绑定VBO
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    //将顶点数据复制到VBO中
    //GL_STATIC_DRAW表示数据不会或几乎不会改变
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    //绑定EBO
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    //将索引数据复制到EBO中
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    //设置顶点属性指针(如何解析顶点数据)
    //第一个参数为顶点属性的位置值，与顶点着色器中的layout(location=0)对应
    //第二个参数为顶点属性的大小，顶点属性是vec3类型，由3个值组成，所以大小为3
    //第三个参数为顶点属性的类型
    //第四个参数为是否希望数据被标准化(0/-1~1)
    //第五个参数为步长，即连续的顶点属性组之间的间隔
    //第六个参数为偏移量，即在缓冲区起始位置的偏移量
    //glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    //启用顶点属性
    //glEnableVertexAttribArray(0);

    //glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
    //glEnableVertexAttribArray(1);

    //glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
    //glEnableVertexAttribArray(2);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    //创建纹理
    glGenTextures(1,&texture1);
    //绑定纹理
    glBindTexture(GL_TEXTURE_2D,texture1);
    //设置纹理环绕方式
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    //设置纹理过滤方式
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    //加载图片，创建纹理并生成mipmap
    int width,height,nrChannels;
    //将图片翻转，因为OpenGL默认y坐标是从下到上的
    stbi_set_flip_vertically_on_load(true);
    //加载图片数据
    unsigned char* data=stbi_load("../../../../assets/container.jpg",&width,&height,&nrChannels,0);
    if(data){
        //生成纹理
        //第一个参数为纹理目标
        //第二个参数为多级渐远纹理级别，0为基本级别
        //第三个参数为纹理存储格式
        //第四、五个参数为纹理宽度、高度
        //第六个参数为历史遗留问题，应该总是为0
        //第七、八个参数为源图格式和数据类型
        //第九个参数为图片数据
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        logger->log(log_level::error,"Failed to load texture");
        throw std::runtime_error("Failed to load texture");
    }
    //释放图片内存
    stbi_image_free(data);
    glGenTextures(1,&texture2);
    glBindTexture(GL_TEXTURE_2D,texture2);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    data=stbi_load("../../../../assets/awesomeface.png",&width,&height,&nrChannels,0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        logger->log(log_level::error,"Failed to load texture");
        throw std::runtime_error("Failed to load texture");
    }
    stbi_image_free(data);

    shader.use();
    shader.set_int("texture1",0);
    shader.set_int("texture2",1);
    shader.unuse();
    
    //解绑VBO
    glBindBuffer(GL_ARRAY_BUFFER,0);
    //解绑VAO
    glBindVertexArray(0);
    //使用线框模式
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

}

void Application::Run(){
    while(!glfwWindowShouldClose(window)){
        ProcessInput();

        Update();

        glClearColor(.2f,.3f,.3f,.0f);
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
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
    glDeleteProgram(shader.get_shader_program_id());
    glfwTerminate();
}

void Application::Update(){
    
}

void Application::Render(){
    //激活纹理单元
    glActiveTexture(GL_TEXTURE0);
    //绑定纹理
    glBindTexture(GL_TEXTURE_2D,texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,texture2);

    shader.use();

    glm::mat4 view=glm::mat4(1.f);
    view=glm::translate(view,glm::vec3(0.f,0.f,-3.f));
    shader.set_mat4("view",glm::value_ptr(view));
    glm::mat4 projection=glm::mat4(1.f);
    projection=glm::perspective(glm::radians(45.f),800.f/600.f,.1f,100.f);
    shader.set_mat4("projection",glm::value_ptr(projection));

    glBindVertexArray(VAO);
    //第一个参数为绘制模式
    //第二个参数为绘制的顶点数量
    //第三个参数为索引的类型
    //第四个参数为EBO中的偏移量
    //glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
    for(int i=0;i<10;++i){
        glm::mat4 model=glm::mat4(1.f);
        model=glm::translate(model,cubePositions[i]);
        float angle=20.f*i;
        model=glm::rotate(model,glm::radians(angle),glm::vec3(1.f,.3f,.5f));
        shader.set_mat4("model",glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES,0,36);
    }
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