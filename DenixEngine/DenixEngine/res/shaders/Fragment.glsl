// DE_SHADER fragment

#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPosition;
out vec4 Color;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

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

struct SpotLight
{
	PointLight Base;
	vec3 Direction;
	float Edge;
};

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

uniform DircetionalLight u_DirLight;

uniform int u_PointLightCount;
uniform PointLight u_PointLight[MAX_POINT_LIGHTS];

uniform int u_SpotLightCount;
uniform SpotLight u_SpotLight[MAX_SPOT_LIGHTS];


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

vec4 CalcPointLight(PointLight _light)
{
	vec3 direction = FragPosition - _light.Position;
	float dist = length(direction);
	direction = normalize(direction);
		
	vec4 Color = CalcLightByDir(_light.Base, direction);
	float attenuation = _light.Exponent * pow(dist,2) + _light.Linear * dist + _light.Constant;
		
	 return (Color / attenuation);
}

vec4 CalcPointLights()
{
	vec4 totalColor = vec4(0, 0, 0, 0);
	for(int i = 0; i < u_PointLightCount; i++)
	{
		totalColor += CalcPointLight(u_PointLight[i]);
	}
	
	return totalColor;
}

vec4 CalcSpotLight(SpotLight _light)
{
	vec3 rayDirction = normalize(FragPosition - _light.Base.Position);
	float spotFactor = dot(rayDirction, normalize(_light.Direction));

	if (spotFactor > _light.Edge)
	{
		vec4 color = CalcPointLight(_light.Base);
		return color * (1.0f - (1.0f - spotFactor) * (1.0f/(1.0f - _light.Edge)));
	}
	else
	{
		return vec4(0.0f);
	}

}

vec4 CalcSpotLights()
{
	vec4 totalColor = vec4(0.0f);
	for(int i = 0; i < u_SpotLightCount; i++)
	{
		totalColor += CalcSpotLight(u_SpotLight[i]);
	}
	
	return totalColor;
}

void main()
{
	vec4 totalColor;

	// Usa
	if (!u_AffectsLighting)
	{
		Color = u_Material.Base.IsTexture ? texture(u_Texture, TexCoord) : vec4(u_Material.Base.Color, 1.0f);
		return;
	}

	// Get color from lighting
	totalColor = CalcDirLight() + CalcPointLights() + CalcSpotLights();

	// Get base from color or texture
	totalColor *= u_Material.Base.IsTexture ? texture(u_Texture, TexCoord) : vec4(u_Material.Base.Color, 1.0f);

	// Output Color
	Color = totalColor;
}