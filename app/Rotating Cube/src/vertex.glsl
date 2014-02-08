#version 150

in  vec4 vPosition;
in  vec4 vColor;
out vec4 color;

uniform vec3 theta;

void main ( ) {
    vec3 angles = radians(theta);
    vec3 Cos = cos(angles);
    vec3 Sin = sin(angles);
    mat4 Rx = mat4(
           1.0,   0.0,   0.0,   0.0,
           0.0, Cos.x,-Sin.x,   0.0,
           0.0, Sin.x, Cos.x,   0.0,
           0.0,   0.0,   0.0,   1.0
    );
    mat4 Ry = mat4(
         Cos.y,   0.0, Sin.y,   0.0,
           0.0,   1.0,   0.0,   0.0,
        -Sin.y,   0.0, Cos.y,   0.0,
           0.0,   0.0,   0.0,   1.0
    );
    mat4 Rz = mat4(
         Cos.z,-Sin.z,   0.0,   0.0,
         Sin.z, Cos.z,   0.0,   0.0,
           0.0,   0.0,   1.0,   0.0,
           0.0,   0.0,   0.0,   1.0
    );
    mat4 rotation = Rx*Ry*Rz;
    
    mat4 camera = mat4( // orthographic
        1.0,0.0,0.0, 0.0,
        0.0,1.0,0.0, 0.0,
        0.0,0.0,1.0,-0.0,
        0.0,0.0,0.0, 1.0
    );
    mat4 orthographic = mat4(
        1.0,0.0,0.0,0.0,
        0.0,1.0,0.0,0.0,
        0.0,0.0,0.0,0.0, // orthographic
        0.0,0.0,0.0,1.0
    );
    gl_Position = orthographic*camera*rotation*vPosition;
    color = vColor;
}
