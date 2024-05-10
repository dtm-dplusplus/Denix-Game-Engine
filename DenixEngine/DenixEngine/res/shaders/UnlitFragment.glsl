// DE_SHADER fragment

#version 330 core

in vec2 TexCoord;
in vec3 Normal;
out vec4 Color;

struct Material
{
	vec3 BaseColor;
	float SpecularIntensity;
	float SpecularPower;
};

uniform sampler2D u_Texture;
uniform bool u_BaseColorAsTexture;

uniform Material u_Material;

void main()
{
	Color = u_BaseColorAsTexture ? vec4(u_Material.BaseColor, 1.0f) : texture(u_Texture, TexCoord);
}