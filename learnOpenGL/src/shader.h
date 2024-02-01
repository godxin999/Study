#pragma once
#include <string>


class Shader{
public:
    Shader()=default;
    Shader(const std::string& vertex_shader_path,const std::string& fragment_shader_path);
    [[nodiscard]]unsigned get_shader_program_id() const;
    void use();
    void set_bool(const std::string& name,bool value) const;
    void set_int(const std::string& name,int value) const;
    void set_float(const std::string& name,float value) const;
    ~Shader()=default;
private:
    unsigned shader_program_id{0};
    void check_compile_errors(unsigned shader,const std::string& type);
};