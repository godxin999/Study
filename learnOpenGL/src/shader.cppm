export module shader;

import <string>;
import glm;

export class Shader{
public:
    Shader()=default;
    Shader(const std::string& vertexShaderPath,const std::string& fragmentShaderPath);
    [[nodiscard]]unsigned GetShaderProgramId() const;
    void Use();
    void Unuse();
    void SetBool(const std::string& name,bool value) const;
    void SetInt(const std::string& name,int value) const;
    void SetFloat(const std::string& name,float value) const;
    void SetVec2(const std::string& name,float x,float y) const;
    void SetVec3(const std::string& name,float x,float y,float z) const;
    void SetVec4(const std::string& name,float x,float y,float z,float w) const;
    void SetVec2(const std::string& name,const glm::vec2& value) const;
    void SetVec3(const std::string& name,const glm::vec3& value) const;
    void SetVec4(const std::string& name,const glm::vec4& value) const;
    void SetMat2(const std::string& name,const glm::mat2& value)const;
    void SetMat3(const std::string& name,const glm::mat3& value)const;
    void SetMat4(const std::string& name,const glm::mat4& value)const;
    ~Shader()=default;
private:
    unsigned m_ShaderProgramId{0};
    void CheckCompileErrors(unsigned shader,const std::string& type);
};