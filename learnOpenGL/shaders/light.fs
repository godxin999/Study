#version 430 core

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform mat4 light;

uniform float matrixLight;
uniform float matrixMove;

vec3 g_Normal;
vec2 g_TexCoords;
vec3 g_ViewDir;
vec4 g_DiffuseTexel;
vec4 g_SpecularTexel;


vec3 BlinnPhong(vec3 lightDir,vec3 lightColor,float luminosity){
    const vec3 halfwayDir=normalize(lightDir+g_ViewDir);
    const float diffuseCoefficient=max(dot(g_Normal,lightDir),0.0);
    const float specularCoefficient=pow(max(dot(g_Normal,halfwayDir),0.0),material.shininess);

    return luminosity*lightColor*(g_DiffuseTexel.rgb*diffuseCoefficient+g_SpecularTexel.rgb*specularCoefficient);
}

float CalculateLuminosity(mat4 LightData){
    const vec3 lightPos=LightData[0].xyz;
    const float constant=LightData[3].x;
    const float linear=LightData[3].y;
    const float quadratic=LightData[3].z;
    const float intensity=LightData[3][3];
    const float distance=length(lightPos-FragPos);
    return intensity/(constant+linear*distance+quadratic*(distance*distance));
}

vec3 CalculatePointLight(mat4 LightData){
    const vec3 lightPos=LightData[0].xyz;
    const vec3 lightColor=LightData[2].rgb;
    const vec3 lightDir=normalize(lightPos-FragPos);
    const float luminosity=CalculateLuminosity(LightData);
    return BlinnPhong(lightDir,lightColor,luminosity);
}

vec3 CalculateDirectionalLight(mat4 LightData){
    const vec3 lightDir=normalize(-LightData[1].xyz);
    const vec3 lightColor=LightData[2].rgb;
    const float luminosity=LightData[3][3];
    return BlinnPhong(lightDir,lightColor,luminosity);
}

vec3 CalculateSpotLight(mat4 LightData){
    const vec3 lightPos=LightData[0].xyz;
    const vec3 SpotDir=normalize(-LightData[1].xyz);
    const vec3 lightColor=LightData[2].rgb;
    const vec3 lightDir=normalize(lightPos-FragPos);
    const float luminosity=CalculateLuminosity(LightData);

    const float cutoff=cos(radians(LightData[1][3]));
    const float outerCutoff=cos(radians(LightData[2][3]));
    
    const float theta=dot(lightDir,SpotDir);
    const float epsilon=cutoff-outerCutoff;
    const float spotIntensity=clamp((theta-outerCutoff)/epsilon,0.0,1.0);

    return BlinnPhong(lightDir,lightColor,luminosity*spotIntensity);
}

void main(){
    g_Normal=normalize(Normal);
    g_TexCoords=TexCoords;
    g_ViewDir=normalize(viewPos-FragPos);
    g_DiffuseTexel=texture(material.diffuse,TexCoords);
    g_SpecularTexel=texture(material.specular,TexCoords);
    switch(int(light[0][3])){
        case 0:
            FragColor=vec4(CalculateDirectionalLight(light),1.0);
            break;
        case 1:
            FragColor=vec4(CalculatePointLight(light),1.0);
            break;
        case 2:
            FragColor=vec4(CalculateSpotLight(light),1.0);
            break;
    }
}