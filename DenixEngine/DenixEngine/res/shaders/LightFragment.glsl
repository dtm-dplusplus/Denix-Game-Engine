#version 330 core

uniform vec4 u_Color;

varying vec3 v_Normal;
varying vec3 v_FragPos;

vec3 lightPos = vec3(10, 10, 10);
vec3 diffuseColor = vec3(1, 1, 1);

void main()
{
	vec3 N = normalize(v_Normal);
	vec3 lightDir = normalize(lightPos - v_FragPos);
	float diff = max(dot(N, lightDir), 0.0);
	vec3 diffuse = diffuseColor * diff;
	vec3 lighting = diffuse;

	gl_FragColor = vec4(lighting, 1) * u_Color;
} 