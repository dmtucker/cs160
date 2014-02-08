#version 330

in vec4 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec4 color;
in vec2 texture;
in vec4 normal;

out vec4 vertex_position;
out vec4 vertex_color;
out vec2 vertex_texture;
out vec4 vertex_normal;

void main ( ) {
    vertex_position = position;
    vertex_color    = color;
    vertex_normal   = normal;
    vertex_texture  = texture;
    gl_Position = projection*view*model*position;
}
