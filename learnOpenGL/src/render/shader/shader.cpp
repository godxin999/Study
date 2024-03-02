module;
#include <glad/glad.h>
module shader;

import stl;
import glm;


void Shader::CheckCompileErrors(uint32_t shader,const std::string& type){
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

Shader::Shader(const std::string& vertexShaderPath,const std::string& fragmentShaderPath){
    std::string vertex_shader_code;
    std::string fragment_shader_code;
    std::ifstream vertex_shader_file;
    std::ifstream fragment_shader_file;
    //保证ifstream对象可以抛出异常
    vertex_shader_file.exceptions(std::ifstream::failbit|std::ifstream::badbit);
    fragment_shader_file.exceptions(std::ifstream::failbit|std::ifstream::badbit);
    try{
        vertex_shader_file.open(vertexShaderPath);
        fragment_shader_file.open(fragmentShaderPath);
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
    CheckCompileErrors(vertex_shader,"VERTEX");
    //创建 fragment shader
    fragment_shader=glCreateShader(GL_FRAGMENT_SHADER);
    //将源码附加到shader对象上
    glShaderSource(fragment_shader,1,&fragment_shader_code_cstr,nullptr);
    //编译shader
    glCompileShader(fragment_shader);
    //检查编译是否成功
    CheckCompileErrors(fragment_shader,"FRAGMENT");
    //创建shader program
    shaderID=glCreateProgram();
    //将shader对象附加到program上
    glAttachShader(shaderID,vertex_shader);
    glAttachShader(shaderID,fragment_shader);
    //链接program
    glLinkProgram(shaderID);
    //检查链接是否成功
    CheckCompileErrors(shaderID,"PROGRAM");
    //删除shader对象
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Shader::Bind(){
    glUseProgram(shaderID);
}

void Shader::Unbind(){
    glUseProgram(0);
}

void Shader::SetBool(const std::string& name,bool value) const{
    glUniform1i(glGetUniformLocation(shaderID,name.c_str()),static_cast<int>(value));
}

void Shader::SetInt(const std::string& name,int value) const{
    glUniform1i(glGetUniformLocation(shaderID,name.c_str()),value);

}

void Shader::SetFloat(const std::string& name,float value) const{
    glUniform1f(glGetUniformLocation(shaderID,name.c_str()),value);
}

void Shader::SetVec2(const std::string& name,float x,float y) const{
    glUniform2f(glGetUniformLocation(shaderID,name.c_str()),x,y);
}

void Shader::SetVec3(const std::string& name,float x,float y,float z) const{
    glUniform3f(glGetUniformLocation(shaderID,name.c_str()),x,y,z);
}

void Shader::SetVec4(const std::string& name,float x,float y,float z,float w) const{
    glUniform4f(glGetUniformLocation(shaderID,name.c_str()),x,y,z,w);
}

void Shader::SetVec2(const std::string& name,const glm::vec2& value) const{
    glUniform2fv(glGetUniformLocation(shaderID,name.c_str()),1,&value[0]);
}

void Shader::SetVec3(const std::string& name,const glm::vec3& value) const{
    glUniform3fv(glGetUniformLocation(shaderID,name.c_str()),1,&value[0]);
}

void Shader::SetVec4(const std::string& name,const glm::vec4& value) const{
    glUniform4fv(glGetUniformLocation(shaderID,name.c_str()),1,&value[0]);
}

void Shader::SetMat2(const std::string& name,const glm::mat2& value)const{
    glUniformMatrix2fv(glGetUniformLocation(shaderID,name.c_str()),1,GL_FALSE,&value[0][0]);
}

void Shader::SetMat3(const std::string& name,const glm::mat3& value)const{
    glUniformMatrix3fv(glGetUniformLocation(shaderID,name.c_str()),1,GL_FALSE,&value[0][0]);
}

void Shader::SetMat4(const std::string& name,const glm::mat4& value)const{
    glUniformMatrix4fv(glGetUniformLocation(shaderID,name.c_str()),1,GL_FALSE,&value[0][0]);
}