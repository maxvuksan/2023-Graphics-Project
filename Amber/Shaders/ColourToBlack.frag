#version 130

uniform sampler2D _texture;

void main()
{
    vec4 samp = texture(_texture, gl_TexCoord[0].xy);
    
    gl_FragColor = vec4(floor(samp.r), floor(samp.g), floor(samp.b), samp.a);
}