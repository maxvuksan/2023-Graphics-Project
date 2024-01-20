// restricts a textures colours to band thresholds

uniform vec2 u_texture_pixel_step; 
uniform sampler2D texture;

void main()
{

    vec4 sprite_colour = texture(texture, gl_TexCoord[0].xy);
    
    float alpha = texture(texture, gl_TexCoord[0].xy + vec2(u_texture_pixel_step.x, 0) ).a;
    alpha = alpha + texture(texture, gl_TexCoord[0].xy + vec2(-u_texture_pixel_step.x, 0) ).a;
    alpha = alpha + texture(texture, gl_TexCoord[0].xy + vec2(0, u_texture_pixel_step.y) ).a;
    alpha = alpha + texture(texture, gl_TexCoord[0].xy + vec2(0, -u_texture_pixel_step.y) ).a;

	vec4 final_color = mix(vec4(1.0,1.0,1.0,1.0), sprite_colour, floor(alpha / 4.0));
	gl_FragColor = vec4(final_color.rgb, sprite_colour.a);
}