// blurs a texture on the y axis, by the u_strength uniform

uniform vec2 u_texture_pixel_step; 
uniform sampler2D texture;
uniform float u_strength; 

void main()
{
    float N = 2 * u_strength + 1;

    vec4 sum = vec4(0.0);

    for (float i = -N; i < N; ++i)
    {
        vec2 tc = gl_TexCoord[0].xy + u_texture_pixel_step * vec2(0, i);
        vec4 samp = texture(texture, tc);
        sum += samp; 
    }

    gl_FragColor = sum / (N * 2);
}