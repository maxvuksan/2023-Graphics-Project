#version 130

uniform vec2 u_light_position;
uniform float u_strength;
uniform float u_radius;
uniform vec4 u_colour;

void main(){

    vec2 pos = gl_TexCoord[0].xy;
    float dis = distance(pos, u_light_position) * u_radius;

    dis -= u_strength;

    float dif = 1.0 - dis;


    gl_FragColor = u_colour * dif;
}