#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main(){
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos- FragPos);//从片段指向光源的向量
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos-FragPos);//从片段指向观察者的向量
    vec3 reflectDir = reflect(-lightDir,norm);//第一个参数要求光源指向片段的向量，第二个参数是法向量，返回的是反射向量
    float spec = pow(max(dot(viewDir,reflectDir),0.0),32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient+diffuse+specular)*objectColor;
    FragColor = vec4(result, 1.0);
}