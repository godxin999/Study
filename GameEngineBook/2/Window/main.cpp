#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


int main(){
    GLFWwindow* window;

    //初始化glfw
    if(!glfwInit()){
        return -1;
    }

    //创建一个Window和OpenGL上下文
    window=glfwCreateWindow(960,640,"Hello World",nullptr,nullptr);
    if(!window){
        glfwTerminate();//创建失败就退出
        return -1;
    }

    //激活上面的OpenGL上下文
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);

    //进入游戏引擎主循环
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glClearColor(49.f/255,77.f/255,121.f/255,1.f);

        glfwSwapBuffers(window);
        //处理键鼠事件
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}