#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPosition;
out vec4 Color;

const int MAX_POINT_LIGHTS = 3;

struct Light
{
	vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;
};

struct DircetionalLight
{
	Light Base;
	vec3 Direction;
};	

struct PointLight
{
	Light Base;
	vec3 Position;
	float Constant;
	float Linear;
	float Exponent;
};

struct Material
{
	vec3 BaseColor;
	float SpecularIntensity;
	float SpecularPower;
};

uniform DircetionalLight u_DirLight;

uniform int u_PointLightCount;
uniform PointLight u_PointLight[MAX_POINT_LIGHTS];



uniform sampler2D u_Texture;
uniform bool u_AffectsLighting;
uniform bool u_BaseColorAsTexture;

uniform Material u_Material;
uniform vec3 u_CameraPosition;

vec4 CalcLightByDir(Light _light, vec3 _direction)
{
	vec4 ambientColor = vec4(_light.Color, 1.0f) * _light.AmbientIntensity;
	
	float diffuseFactor = max(dot(normalize(Normal), normalize(_direction)), 0.0f);
	vec4 diffuseColor = vec4(_light.Color * _light.DiffuseIntensity * diffuseFactor, 1.0f);
	
	vec4 specularColor = vec4(0, 0, 0, 0);
	
	if(diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(u_CameraPosition - FragPosition);
		vec3 reflectedVertex = normalize(reflect(_direction, normalize(Normal)));
		
		float specularFactor = dot(fragToEye, reflectedVertex);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, u_Material.SpecularPower);
			specularColor = vec4(_light.Color * u_Material.SpecularIntensity * specularFactor, 1.0f);
		}
	}

	return (ambientColor + diffuseColor + specularColor);
}

vec4 CalcDirLight()
{
	return CalcLightByDir(u_DirLight.Base, u_DirLight.Direction);

}

vec4 CalcPointLights()
{
	vec4 totalColor = vec4(0, 0, 0, 0);
	for(int i = 0; i < u_PointLightCount; i++)
	{
		vec3 direction = FragPosition - u_PointLight[i].Position;
		float dist = length(direction);
		direction = normalize(direction);
		
		vec4 Color = CalcLightByDir(u_PointLight[i].Base, direction);
		float attenuation = u_PointLight[i].Exponent * pow(dist,2) + u_PointLight[i].Linear * dist + u_PointLight[i].Constant;
		
		totalColor += (Color / attenuation);
	}
	
	return totalColor;
}

void main()
{
	// This is slow. Will swap shaders instead later.
	if (!u_AffectsLighting)
	{
		Color = texture(u_Texture, TexCoord) * vec4(u_Material.BaseColor, 1.0f);
		return;
	}

	// Get color from lighting
	vec4 totalColor = CalcDirLight() + CalcPointLights();

	// Get base from color or texture
	if(!u_BaseColorAsTexture)
	{
		totalColor *= texture(u_Texture, TexCoord);
	}
	else
	{
		totalColor *= vec4(u_Material.BaseColor, 1.0f);
	}

	// Output Color
	Color = totalColor;
}