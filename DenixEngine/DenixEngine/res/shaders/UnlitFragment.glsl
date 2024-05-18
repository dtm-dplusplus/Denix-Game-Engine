// DE_SHADER fragment

#version 330 core

in vec2 TexCoord;
in vec3 Normal;
out vec4 Color;

struct BaseMatParam
{
	vec3 Color;
	bool IsTexture;
};

struct Material
{
	BaseMatParam Base;
	float SpecularIntensity;
	float SpecularPower;
};

uniform sampler2D u_Texture;

uniform Material u_Material;

void main()
{
	Color = u_Material.Base.IsTexture ? texture(u_Texture, TexCoord) : vec4(u_Material.Base.Color, 1.0f);
}