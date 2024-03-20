#version 130

uniform vec4 u_colour;
uniform float u_blend;
uniform sampler2D texture;

void main(){

    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    gl_FragColor = pixel * vec4(1.0 - u_blend) + vec4(u_colour.r, u_colour.g, u_colour.b, 1.0) * vec4(u_blend);
}