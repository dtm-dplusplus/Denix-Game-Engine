// DE_SHADER vertex

#version 330 core
layout (location = 0) in vec4 a_Pos;

out vec2 TexCoords;

uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * vec4(a_Pos.xy, 0.0, 1.0);
    TexCoords = a_Pos.zw;
}  