#version 330

layout(location=0) in vec4 position;
layout(location=1) in vec4 color;
out vec4 vColor;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

void main ( ) {
    //gl_Position = (projection*view*model)*position;
    gl_Position = position;
    vColor = color;
}
