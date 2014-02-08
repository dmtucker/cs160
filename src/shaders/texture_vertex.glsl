#version 330

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec4 position;
in vec4 color;
in vec2 texture_coordinate;

out vec4 vertex_color;
out vec2 vertex_texture;

void main ( ) {
    gl_Position    = projection*view*model*position;
    vertex_color   = color;
    vertex_texture = texture_coordinate;
}
