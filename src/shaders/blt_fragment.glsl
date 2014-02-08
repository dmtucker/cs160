#version 330

in vec4 vertex_color;
in vec2 vertex_texture;

uniform sampler2D texture_map;

out vec4 fragment_color;

void main ( ) { fragment_color = vertex_color*texture(texture_map,vertex_texture); }
