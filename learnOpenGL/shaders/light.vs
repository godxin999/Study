#version 430 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTexCoords;

out vec3 Normal;
out vec3 FragWorldPos;
out vec3 FragViewPos;
out vec2 TexCoords;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transInvModel;

void main(){
    Normal=mat3(transInvModel)*aNormal;
    FragWorldPos=vec3(model*vec4(aPos,1.0));
    FragViewPos=vec3(view*vec4(FragWorldPos,1.0));
    TexCoords=aTexCoords;
    gl_Position=projection*view*model*vec4(aPos,1.0);
}