#include "application.h"
#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>



float vertices[]={
    .5f,.5f,0.f,0.f,0.f,0.f,//右上角
    .5f,-.5f,0.f,1.f,0.f,0.f,//右下角
    -.5f,-.5f,0.f,0.f,1.f,0.f,//左下角
    -.5f,.5f,0.f,0.f,0.f,1.f//左上角
};

unsigned indices[]={
    0,1,3,
    1,2,3
};

unsigned VAO,VBO,EBO;
Shader shader;

//窗口大小改变时的回调函数
void framebuffer_size_callback(GLFWwindow* window,int width,int height){
    glViewport(0,0,width,height);//前两个参数为窗口的左下角位置，后两个参数为渲染窗口的宽高
}

void Application::Init(){
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
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);//注册窗口大小改变的回调函数

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        throw std::runtime_error("Failed to initialize GLAD");
    }

    shader=Shader("../../../../shaders/test.vs","../../../../shaders/test.fs");

    //创建VAO
    glGenVertexArrays(1,&VAO);
    //创建VBO
    glGenBuffers(1,&VBO);
    //创建EBO
    glGenBuffers(1,&EBO);
    //绑定VAO
    glBindVertexArray(VAO);
    //绑定VBO
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    //将顶点数据复制到VBO中
    //GL_STATIC_DRAW表示数据不会或几乎不会改变
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    //绑定EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    //将索引数据复制到EBO中
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    //设置顶点属性指针(如何解析顶点数据)
    //第一个参数为顶点属性的位置值，与顶点着色器中的layout(location=0)对应
    //第二个参数为顶点属性的大小，顶点属性是vec3类型，由3个值组成，所以大小为3
    //第三个参数为顶点属性的类型
    //第四个参数为是否希望数据被标准化(0/-1~1)
    //第五个参数为步长，即连续的顶点属性组之间的间隔
    //第六个参数为偏移量，即在缓冲区起始位置的偏移量
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
    //启用顶点属性
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);


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
        glClear(GL_COLOR_BUFFER_BIT);//清除颜色缓冲
        
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

    shader.use();
    glBindVertexArray(VAO);
    //第一个参数为绘制模式
    //第二个参数为绘制的顶点数量
    //第三个参数为索引的类型
    //第四个参数为EBO中的偏移量
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
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