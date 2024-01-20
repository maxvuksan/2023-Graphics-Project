// converts any non white values to black

uniform sampler2D texture;

void main()
{
    vec4 samp = texture(texture, gl_TexCoord[0].xy);
    
    gl_FragColor = vec4(floor(samp.r), floor(samp.g), floor(samp.b), samp.a);
}