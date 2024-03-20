#version 130

uniform vec2 u_texture_pixel_step; 
uniform sampler2D _texture;
uniform float u_strength; 

void main()
{
    float N = 2.0 * u_strength + 1.0;

    vec4 sum = vec4(0.0);

    for (float i = -N; i < N; ++i)
    {
        vec2 tc = gl_TexCoord[0].xy + u_texture_pixel_step * vec2(0.0, i);
        vec4 samp = texture(_texture, tc);
        sum += samp; 
    }

    gl_FragColor = sum / (N * 2.0);
}