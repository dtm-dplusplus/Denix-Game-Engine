// DE_SHADER fragment

#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPosition;
out vec4 Color;

struct Material
{
	vec3 BaseColor;
	bool IsBaseTexture;
	float SpecularIntensity;
	float SpecularPower;
};

uniform sampler2D u_Texture;

uniform Material u_Material;
uniform vec3 u_CameraPosition;

void main()
{
	vec4 totalColor = u_Material.IsBaseTexture ? texture(u_Texture, TexCoord) : vec4(u_Material.BaseColor, 1.0f);

	// Output Color
	Color = totalColor;
}