export module converter;

import glm;
import imgui;

namespace Engine::inline UI{
    export class Converter{
    public:
        Converter()=delete;
        ~Converter()=delete;
        static glm::vec2 ToVec2(const ImVec2& vec);
        static ImVec2 ToImVec2(const glm::vec2& vec);
        static glm::vec4 ToVec4(const ImVec4& vec);
        static ImVec4 ToImVec4(const glm::vec4& vec);
    };
}

module : private;

namespace Engine::inline UI{
    glm::vec2 Converter::ToVec2(const ImVec2& vec){
        return glm::vec2(vec.x, vec.y);
    }

    ImVec2 Converter::ToImVec2(const glm::vec2& vec){
        return ImVec2(vec.x, vec.y);
    }

    glm::vec4 Converter::ToVec4(const ImVec4& vec){
        return glm::vec4(vec.x, vec.y, vec.z, vec.w);
    }
    
    ImVec4 Converter::ToImVec4(const glm::vec4& vec){
        return ImVec4(vec.x, vec.y, vec.z, vec.w);
    }
}