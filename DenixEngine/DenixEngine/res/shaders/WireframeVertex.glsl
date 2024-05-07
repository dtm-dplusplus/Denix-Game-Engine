// DE_SHADER vertex

#version 330 core
layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_TexCoord;
layout (location = 2) in vec3 in_Normal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPosition;

uniform mat4 u_Model;
uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(in_Position, 1.0);
	
	TexCoord = in_TexCoord;
	Normal = in_Normal;
	FragPosition = vec3(u_Model * vec4(in_Position, 1.0));
}