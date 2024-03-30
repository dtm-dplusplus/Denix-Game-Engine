#version 330 core

in vec2 TexCoord;

out vec4 colour;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

void main()
{
	colour = texture(u_Texture, TexCoord) * u_Color;
}