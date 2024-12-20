module light;

import glm;
import stl;
import transform;


float CalculatePointLightRange(float intensity,const glm::vec3& attCoeff){    
    float threshold=85.f*intensity;
    float discriminant=attCoeff.y*attCoeff.y-4.f*attCoeff.z*(attCoeff.x-threshold);
    if(discriminant<0.f){
        return 0.f;
    }
    float range=(-attCoeff.y+std::sqrt(discriminant))/(2.f*attCoeff.z);
    if(range>1000.f)return std::numeric_limits<float>::infinity();   
    return range;
}

namespace Engine::inline Render{
    glm::mat4 Light::GenerateDataMatrix()const{
        glm::mat4 result;
        auto position=m_transform.GetWorldPosition();
        result[0]=glm::vec4(position,type);
        auto forward=m_transform.GetWorldForward();
        result[1]=glm::vec4(forward,cutoff);
        result[2]=glm::vec4(color,outerCutoff);
        result[3]=glm::vec4(attCoeff,intensity);
        return result;
    }

    float Light::GetEffectRange()const{
        switch(static_cast<LightType>(static_cast<int>(type))){
            case LightType::Spot:
                [[fallthrough]];
            case LightType::Point:
                return CalculatePointLightRange(intensity,attCoeff);
            case LightType::AmbientSphere:
                return attCoeff.x;
            default:
                return std::numeric_limits<float>::infinity();
        }
    }
}