#version 130

uniform sampler2D _texture;
uniform float u_band_count; 

void main()
{
    vec4 samp = texture(_texture, gl_TexCoord[0].xy);
    
    vec4 color = round((samp - 0.3) * u_band_count) / u_band_count;
    color.a = 0.3;
    gl_FragColor = color;
}