#version 430 core

layout (std140) uniform UBO
{
    //mat4 model;
    mat4 view;
    mat4 projection;
    //mat4 transInvModel;
    vec3 viewPos;
};

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

layout (std430, binding = 0) buffer LightSSBO
{
    mat4 lights[];
};

uniform float u_Shininess=100.0;
uniform vec4 u_Diffuse=vec4(1.0,1.0,1.0,1.0);
uniform vec3 u_Specular=vec3(1.0,1.0,1.0);
uniform sampler2D u_DiffuseMap;
uniform sampler2D u_SpecularMap;


out vec4 FragColor;

vec3 g_Normal;
vec2 g_TexCoords;
vec3 g_ViewDir;
vec4 g_DiffuseTexel;
vec4 g_SpecularTexel;

vec3 BlinnPhong(vec3 lightDir,vec3 lightColor,float luminosity){
    const vec3 halfwayDir=normalize(lightDir+g_ViewDir);
    const float diffuseCoefficient=max(dot(g_Normal,lightDir),0.0);
    const float specularCoefficient=pow(max(dot(g_Normal,halfwayDir),0.0),u_Shininess);

    return luminosity*lightColor*(g_DiffuseTexel.rgb*diffuseCoefficient+g_SpecularTexel.rgb*specularCoefficient);
}

float CalculateLuminosity(mat4 LightData){
    const vec3 lightPos=LightData[0].xyz;
    const float constant=LightData[3].x;
    const float linear=LightData[3].y;
    const float quadratic=LightData[3].z;
    const float intensity=LightData[3][3];
    const float distance=length(lightPos-fs_in.FragPos);
    return intensity/(constant+linear*distance+quadratic*(distance*distance));
}

vec3 CalculatePointLight(mat4 LightData){
    const vec3 lightPos=LightData[0].xyz;
    const vec3 lightColor=LightData[2].rgb;
    const vec3 lightDir=normalize(lightPos-fs_in.FragPos);
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
    const vec3 lightDir=normalize(lightPos-fs_in.FragPos);
    const float luminosity=CalculateLuminosity(LightData);

    const float cutoff=cos(radians(LightData[1][3]));
    const float outerCutoff=cos(radians(LightData[2][3]));
    
    const float theta=dot(lightDir,SpotDir);
    const float epsilon=cutoff-outerCutoff;
    const float spotIntensity=clamp((theta-outerCutoff)/epsilon,0.0,1.0);

    return BlinnPhong(lightDir,lightColor,luminosity*spotIntensity);
}

vec3 CalculateAmbientSphereLight(mat4 LightData){
    const vec3 lightPos=LightData[0].xyz;
    const vec3 lightColor=LightData[2].rgb;
    const float intensity=LightData[3][3];
    const float radius=LightData[3].x;
    return distance(lightPos,fs_in.FragPos)<=radius?g_DiffuseTexel.rgb*lightColor*intensity:vec3(0.0);
}

void main(){
    g_TexCoords=fs_in.TexCoords;
    g_Normal=fs_in.Normal;
    g_ViewDir=normalize(viewPos-fs_in.FragPos);
    g_DiffuseTexel=texture(u_DiffuseMap,fs_in.TexCoords)*u_Diffuse;
    g_SpecularTexel=texture(u_SpecularMap,fs_in.TexCoords)*vec4(u_Specular,1.0);

    vec3 lightSum=vec3(0.0);
    for(int i=0;i<lights.length();++i){
        switch(int(lights[i][0][3])){
        case 0:
            lightSum+=CalculateDirectionalLight(lights[i]);
            break;
        case 1:
            lightSum+=CalculatePointLight(lights[i]);
            break;
        case 2:
            lightSum+=CalculateSpotLight(lights[i]);
            break;
        case 3:
            lightSum+=CalculateAmbientSphereLight(lights[i]);
            break;
        }
    }
    FragColor=vec4(lightSum,g_DiffuseTexel.a);
    //FragColor=vec4(1.0);
}