/*************************************************************************
	> File Name: main.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/25 22:30:06
 ************************************************************************/

#include <iostream>
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <stdlib.h>
#include <stdio.h>

#include "VertexData.h"
#include "ShaderSource.h"


static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error:%s\n", description);
}

GLFWwindow* window;
GLuint vertex_shader, fragment_shader, program;
GLint mvp_location, vpos_location, vcol_location;

void init_opengl() {
    //���ô���ص�����
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    //��������
    window = glfwCreateWindow(960, 640, "Triangle", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);
}

void compile_shader() {
    //����������ɫ��
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    //ָ��shaderԴ��
    glShaderSource(vertex_shader, 1, &vertex_shader_text, nullptr);
    //����shader
    glCompileShader(vertex_shader);
    //��ȡ������
    GLint compile_status = GL_FALSE;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status == GL_FALSE) {
        GLchar message[256];
        glGetShaderInfoLog(vertex_shader, sizeof(message), 0, message);
        std::cout << "compile error:" << message << std::endl;
    }

    //����Ƭ����ɫ��
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    //ָ��shaderԴ��
    glShaderSource(fragment_shader, 1, &fragment_shader_text, nullptr);
    //����shader
    glCompileShader(fragment_shader);
    //��ȡ������
    compile_status = GL_FALSE;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status == GL_FALSE) {
        GLchar message[256];
        glGetShaderInfoLog(fragment_shader, sizeof(message), 0, message);
        std::cout << "compile error:" << message << std::endl;
    }

    //����GPU����
    program = glCreateProgram();
    //����shader
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    //����
    glLinkProgram(program);
    //��ȡ������
    GLint link_status = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE) {
        GLchar message[256];
        glGetProgramInfoLog(program, sizeof(message), 0, message);
        std::cout << "link error:" << message << std::endl;
    }
}

int main() {

    init_opengl();
    compile_shader();

    //��ȡshader������ID
    mvp_location = glGetUniformLocation(program, "u_mvp");
    vpos_location = glGetAttribLocation(program, "a_pos");
    vcol_location = glGetAttribLocation(program, "a_color");

    while (!glfwWindowShouldClose(window)) {
        float ratio;
        int width, height;
        glm::mat4 m, v, p, mvp;

        //��ȡ������
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(49.f / 255, 77.f / 255, 121.f / 255, 1.f);

        //����任
        glm::mat4 trans = glm::translate(glm::vec3(0, 0, 0));//���ƶ���������
        glm::mat4 ratation = glm::eulerAngleYXZ(glm::radians(0.f), glm::radians(0.f), glm::radians(0.f));//ŷ����
        glm::mat4 scale = glm::scale(glm::vec3(3.f, 3.f, 3.f));

        m = trans * scale * ratation;
        v = glm::lookAt(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        p = glm::perspective(glm::radians(60.f), ratio, 1.f, 1000.f);

        mvp = p * v * m;

        //ָ��GPU����
        glUseProgram(program);
        {
            //���ö�����ɫ������(a_pos)��ָ���붥���������ݽ��й���
            glEnableVertexAttribArray(vpos_location);
            glVertexAttribPointer(vpos_location, 3, GL_FLOAT, false, sizeof(glm::vec3), kPos);

            //���ö�����ɫ������(a_color)��ָ���붥����ɫ���ݽ��й���
            glEnableVertexAttribArray(vcol_location);
            glVertexAttribPointer(vcol_location, 3, GL_FLOAT, false, sizeof(glm::vec4), kColors);

            //�ϴ�mvp����
            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &mvp[0][0]);

            //�ϴ��������ݲ����л���
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);

    return 0;
}