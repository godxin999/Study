#include "shader.h"
#include <fstream>
#include <ostream>
#include <sstream>
#include <iostream>
#include <format>

#include <glad/glad.h>

void Shader::check_compile_errors(unsigned shader,const std::string& type){
    int success;
    char info_log[512];
    if(type!="PROGRAM"){
        glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
        if(!success){
            glGetShaderInfoLog(shader,512,nullptr,info_log);
            std::cout<<std::format("ERROR::SHADER_COMPILATION_ERROR of type: {}\n{}\n",type,info_log);
            throw std::runtime_error(std::format("ERROR::SHADER_COMPILATION_ERROR of type: {}\n{}\n",type,info_log));
        }
    }
    else{
        glGetProgramiv(shader,GL_LINK_STATUS,&success);
        if(!success){
            glGetProgramInfoLog(shader,512,nullptr,info_log);
            throw std::runtime_error(std::format("ERROR::PROGRAM_LINKING_ERROR of type: {}\n{}\n",type,info_log));
        }
    }
}

Shader::Shader(const std::string& vertex_shader_path,const std::string& fragment_shader_path){
    std::string vertex_shader_code;
    std::string fragment_shader_code;
    std::ifstream vertex_shader_file;
    std::ifstream fragment_shader_file;
    //保证ifstream对象可以抛出异常
    vertex_shader_file.exceptions(std::ifstream::failbit|std::ifstream::badbit);
    fragment_shader_file.exceptions(std::ifstream::failbit|std::ifstream::badbit);
    try{
        vertex_shader_file.open(vertex_shader_path);
        fragment_shader_file.open(fragment_shader_path);
        std::stringstream vertex_shader_stream,fragment_shader_stream;
        //读取文件的缓冲内容到数据流中
        vertex_shader_stream<<vertex_shader_file.rdbuf();
        fragment_shader_stream<<fragment_shader_file.rdbuf();
        vertex_shader_file.close();
        fragment_shader_file.close();
        vertex_shader_code=vertex_shader_stream.str();
        fragment_shader_code=fragment_shader_stream.str();
    }
    catch(std::ifstream::failure& e){
        std::cerr<<std::format("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n{}\n",e.what());
    }
    const char* vertex_shader_code_cstr=vertex_shader_code.c_str();
    const char* fragment_shader_code_cstr=fragment_shader_code.c_str();
    unsigned vertex_shader,fragment_shader;
    //创建 vertex shader
    vertex_shader=glCreateShader(GL_VERTEX_SHADER);
    //将源码附加到shader对象上
    glShaderSource(vertex_shader,1,&vertex_shader_code_cstr,nullptr);//第二个参数为字符串数量
    //编译shader
    glCompileShader(vertex_shader);
    //检查编译是否成功
    check_compile_errors(vertex_shader,"VERTEX");
    //创建 fragment shader
    fragment_shader=glCreateShader(GL_FRAGMENT_SHADER);
    //将源码附加到shader对象上
    glShaderSource(fragment_shader,1,&fragment_shader_code_cstr,nullptr);
    //编译shader
    glCompileShader(fragment_shader);
    //检查编译是否成功
    check_compile_errors(fragment_shader,"FRAGMENT");
    //创建shader program
    shader_program_id=glCreateProgram();
    //将shader对象附加到program上
    glAttachShader(shader_program_id,vertex_shader);
    glAttachShader(shader_program_id,fragment_shader);
    //链接program
    glLinkProgram(shader_program_id);
    //检查链接是否成功
    check_compile_errors(shader_program_id,"PROGRAM");
    //删除shader对象
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

unsigned Shader::get_shader_program_id()const{
    return shader_program_id;
}

void Shader::use(){
    glUseProgram(shader_program_id);
}

void Shader::set_bool(const std::string& name,bool value) const{
    glUniform1i(glGetUniformLocation(shader_program_id,name.c_str()),static_cast<int>(value));
}

void Shader::set_int(const std::string& name,int value) const{
    glUniform1i(glGetUniformLocation(shader_program_id,name.c_str()),value);

}

void Shader::set_float(const std::string& name,float value) const{
    glUniform1f(glGetUniformLocation(shader_program_id,name.c_str()),value);
}
