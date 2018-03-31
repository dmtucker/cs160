#version 330

in  vec4 vPosition;
in  vec4 vColor;
out vec4 color;

uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;

void main ( ) {
    gl_Position = vProjection*vView*vModel*vPosition;
    color = vColor;
}
