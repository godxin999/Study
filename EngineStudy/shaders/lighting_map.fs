#version 330 core

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

uniform float matrixLight;
uniform float matrixMove;

void main(){
    vec3 ambient=light.ambient*vec3(texture(material.diffuse,TexCoords));

    vec3 norm=normalize(Normal);
    vec3 lightDir=normalize(light.position-FragPos);
    float diff=max(dot(norm,lightDir),0.0);
    vec3 diffuse=light.diffuse*diff*vec3(texture(material.diffuse,TexCoords));

    vec3 viewDir=normalize(viewPos-FragPos);
    vec3 reflectDir=reflect(-lightDir,norm);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular=light.specular*spec*vec3(texture(material.specular,TexCoords));

    vec3 emission=matrixLight*vec3(texture(material.emission,vec2(TexCoords.x,TexCoords.y+matrixMove)));

    vec3 result=ambient+diffuse+specular+emission;
    FragColor=vec4(result,1.0);
}