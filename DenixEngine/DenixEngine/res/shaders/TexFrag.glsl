uniform sampler2D u_Texture;

varying vec2 v_TexCoord;

void main()
{

  vec4 tex = texture2D(u_Texture, v_TexCoord);
  gl_FragColor = vec4(v_TexCoord, 0.0,1.0);
  gl_FragColor = vec4(1.0,0.0, 0.0,1.0);
}