#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_TexCoord;

out vec4 vCol;
out vec2 TexCoord;

uniform mat4 u_Model;
uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(in_Position, 1.0);
	vCol = vec4(clamp(in_Position, 0.0f, 1.0f), 1.0f);
	
	TexCoord = in_TexCoord;
}