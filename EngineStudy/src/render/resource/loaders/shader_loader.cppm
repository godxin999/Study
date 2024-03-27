module;
#include <glad/glad.h>
export module shader_loader;

import shader;
import stl;


namespace Engine::inline Render{
    export class ShaderLoader{
    public:
        ShaderLoader()=delete;
        ~ShaderLoader()=delete;
        //static Shader* LoadShader(const std::string& vertexShaderPath,const std::string& fragmentShaderPath);
        //static void ReloadShader(Shader& shader,const std::string& vertexShaderPath,const std::string& fragmentShaderPath);
        //static bool UnloadShader(Shader*& shader);
    };
}