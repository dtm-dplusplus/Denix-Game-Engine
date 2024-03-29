#version 330 core

in vec4 vCol;
in vec2 TexCoord;

out vec4 colour;

uniform sampler2D u_Texture;

void main()
{
	colour = texture(u_Texture, TexCoord);
}