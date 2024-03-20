#version 130

uniform vec2 u_texture_pixel_step; 
uniform vec4 u_colour;
uniform sampler2D _texture;

void main()
{

    vec4 sprite_colour = texture(_texture, gl_TexCoord[0].xy);
    
    float alpha = texture(_texture, gl_TexCoord[0].xy + vec2(u_texture_pixel_step.x, 0.0) ).a;
    alpha = alpha + texture(_texture, gl_TexCoord[0].xy + vec2(-u_texture_pixel_step.x, 0.0) ).a;
    alpha = alpha + texture(_texture, gl_TexCoord[0].xy + vec2(0, u_texture_pixel_step.y) ).a;
    alpha = alpha + texture(_texture, gl_TexCoord[0].xy + vec2(0, -u_texture_pixel_step.y) ).a;

	vec4 final_color = mix(sprite_colour, u_colour, ceil(alpha) * floor(1.0 - sprite_colour.a));
	gl_FragColor = vec4(final_color.rgba);
}