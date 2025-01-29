// DE_SHADER fragment

#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPosition;
out vec4 Color;

struct Material
{
	vec3 BaseColor;
	bool MultiplyBase;
	float TextureTiling;
	bool IsBaseTexture;
	float SpecularIntensity;
	float SpecularPower;
};

uniform sampler2D u_Texture;

uniform Material u_Material;

void main()
{
	if(u_Material.IsBaseTexture && u_Material.MultiplyBase)
	{
		vec4 totalColor = texture(u_Texture, TexCoord * u_Material.TextureTiling) * vec4(u_Material.BaseColor, 1.0f);
		Color = totalColor;
		return;
	}

	// Output Color
	Color = u_Material.IsBaseTexture ? texture(u_Texture, TexCoord * u_Material.TextureTiling) : vec4(u_Material.BaseColor, 1.0f);;
}