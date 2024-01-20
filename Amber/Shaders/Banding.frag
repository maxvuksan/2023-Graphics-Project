// restricts a textures colours to band thresholds

uniform sampler2D texture;
uniform float u_band_count; 

void main()
{
    vec4 samp = texture(texture, gl_TexCoord[0].xy);
    
    gl_FragColor = round(samp * u_band_count) / u_band_count;
}