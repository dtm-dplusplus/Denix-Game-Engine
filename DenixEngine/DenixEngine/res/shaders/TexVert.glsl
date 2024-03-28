#version 330 core

layout(location = 0) in vec3 in_Position;
layout(location = 2) in vec2 in_TexCoord;

uniform mat4 u_Projection;
uniform mat4 u_Model;
uniform mat4 u_View;

out vec2 TexCoord;

void main()
{
  gl_Position = u_Projection * u_View * u_Model * vec4(in_Position, 1.0);
  TexCoord = in_TexCoord;
}