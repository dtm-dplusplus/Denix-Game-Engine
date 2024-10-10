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
	bool IsValid;
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

uniform Material u_Material;
uniform vec3 u_CameraPosition;

vec3 CalcDirLightColor(Light _light, vec3 _direction)
{
	vec3 ambientColor = _light.Color * _light.AmbientIntensity;

	// Calc diffuse factor based on light angle, min is 0.0f
	float diffuseFactor = max(dot(normalize(Normal), normalize(_direction)), 0.0f);

	// Calc diffuse color of incident light
	vec3 diffuseColor = _light.Color * _light.DiffuseIntensity * diffuseFactor;

	vec3 specularColor = vec3(0.0f);

	//if(diffuseFactor > 0.0f)
	//{
		//vec3 fragToEye = normalize(u_CameraPosition - FragPosition);
	//	vec3 reflectedVertex = normalize(reflect(_direction, normalize(Normal)));

	//	float specularFactor = dot(fragToEye, reflectedVertex);
		//if(specularFactor > 0.0f)
		//{
	//		specularFactor = pow(specularFactor, u_Material.SpecularPower);
	//		specularColor = vec4(_light.Color * u_Material.SpecularIntensity * specularFactor, 1.0f);
		//}
	//}

	return vec3(ambientColor + diffuseColor + specularColor);
}

vec3 CalcDirLight()
{
	return CalcDirLightColor(u_DirLight.Base, u_DirLight.Direction);

}


// Calculate the color of light reflected from a surfce
// u_Material.Base.Color = diffuse color
// u_DirLight.Base.DiffuseIntensity - Amplifies intensity of light color
// dot(_n, _l),0.0f,1.0f - Determines light intensity with respect to angle of incidence
// clamp(dot(_n, _l),0.0f,1.0f) - clamp ensures we don't get negative illuminance. This can happen if the face is away from dir light
vec3 CalcDiffuseReflectance(vec3 _n,vec3 _l)
{
	vec3 directColor = CalcDirLight();

	return directColor * u_Material.Base.Color;
}


void main()
{
	vec4 totalColor = vec4(0.0f);
	// Usa
	if (!u_AffectsLighting)
	{
		Color = u_Material.Base.IsTexture ? texture(u_Texture, TexCoord) : vec4(u_Material.Base.Color, 1.0f);
		return;
	}


	// Get diffuse from direct sources
	if(u_DirLight.IsValid)
	{

		totalColor += vec4(CalcDiffuseReflectance(Normal, u_DirLight.Direction), 1.0f);
	}



	// Get base from color or texture
	//totalColor *= u_Material.Base.IsTexture ? texture(u_Texture, TexCoord) : vec4(u_Material.Base.Color, 1.0f);

	// Output Color
	Color = totalColor;
}
