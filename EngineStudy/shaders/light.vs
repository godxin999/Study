#version 430 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTexCoords;

layout (std140) uniform UBO
{
    //mat4 model;
    mat4 view;
    mat4 projection;
    //mat4 transInvModel;
    vec3 viewPos;
};

out VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
}vs_out;

uniform mat4 model;
uniform mat4 transInvModel;

void main(){
    vs_out.FragPos=vec3(model*vec4(aPos,1.0));
    vs_out.Normal=normalize(mat3(transInvModel)*aNormal);
    vs_out.TexCoords=aTexCoords;
    gl_Position=projection*view*vec4(vs_out.FragPos,1.0);
}