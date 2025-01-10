// DE_SHADER fragment

#version 330 core

  
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D u_ViewportTexture;

void main()
{ 
    FragColor = texture(u_ViewportTexture, TexCoords);
}