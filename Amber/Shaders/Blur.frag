uniform sampler2D u_texture;
uniform vec2 u_texture_pixel_step; 

uniform float u_strength; 

void main()
{
  float N = 2 * u_strength + 1;

  vec4 sum = vec4(0.0);

  for (float i = 0; i < N; ++i)
  {
    for (float j = 0; j < N; ++j)
    {
      vec2 tc = gl_TexCoord[0].xy + u_texture_pixel_step * vec2(float(i - u_strength), float(j - u_strength));
      sum += texture(u_texture, tc);
    }
  }

  gl_FragColor = sum / (N * N);
}