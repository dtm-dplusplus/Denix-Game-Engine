#version 330 core

uniform sampler2D u_Tex;
  
in vec2 TexCoord;
  
void main()
{
  vec4 tex = texture(u_Tex, TexCoord);
  gl_FragColor = tex;
}
