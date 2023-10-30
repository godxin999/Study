#version 330 core

uniform sampler2D u_diffuse_texture;

in vec4 v_color;
in vec2 v_uv;
layout(location = 0)out vec4 o_fragColor;

void main()
{
    vec4 pixColor=texture(u_diffuse_texture,v_uv);
    //因为字体纹理是GL_RED格式，所以只有R通道有数据，将传入的颜色作为字体的颜色，并将R通道的值作为字体的Alpha值
    o_fragColor=vec4(v_color.x,v_color.y,v_color.z,pixColor.r);
}