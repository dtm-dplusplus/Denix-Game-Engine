#version 330 core

uniform sampler2D u_Texture;
  
in vec2 TexCoord;
  
void main()
{
  vec4 tex = texture2D(u_Texture, TexCoord);
  gl_FragColor = tex;
}
