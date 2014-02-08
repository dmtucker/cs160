#ifndef MATRIX_H
#define MATRIX_H

#define     IDENTITY mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1)
#define ORTHOGRAPHIC mat4(1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1)
#define DEG2RAD (M_PI/180.0)
#define cot(x) tan(M_PI_2-x)

#include <math.h>
#include "Angel.h"

mat4    model = IDENTITY,  view = IDENTITY,  projection = IDENTITY;


mat4 scale ( GLfloat x , GLfloat y , GLfloat z ) {
    return mat4(
        x,0,0,0,
        0,y,0,0,
        0,0,z,0,
        0,0,0,1
    );
}


mat4 translate ( GLfloat x , GLfloat y , GLfloat z ) {
    return mat4(
        1,0,0,x,
        0,1,0,y,
        0,0,1,z,
        0,0,0,1
    );
}


mat4 rotate ( GLfloat x , GLfloat y , GLfloat z ) {
    GLfloat cosX = cos(x*DEG2RAD);
    GLfloat cosY = cos(y*DEG2RAD);
    GLfloat cosZ = cos(z*DEG2RAD);
    GLfloat sinX = sin(x*DEG2RAD);
    GLfloat sinY = sin(y*DEG2RAD);
    GLfloat sinZ = sin(z*DEG2RAD);
    mat4 Rx = mat4(
          1.0,  0.0,  0.0,  0.0,
          0.0, cosX,-sinX,  0.0,
          0.0, sinX, cosX,  0.0,
          0.0,  0.0,  0.0,  1.0
    );
    mat4 Ry = mat4(
         cosY,  0.0, sinY,  0.0,
          0.0,  1.0,  0.0,  0.0,
        -sinY,  0.0, cosY,  0.0,
          0.0,  0.0,  0.0,  1.0
    );
    mat4 Rz = mat4(
         cosZ,-sinZ,  0.0,  0.0,
         sinZ, cosZ,  0.0,  0.0,
          0.0,  0.0,  1.0,  0.0,
          0.0,  0.0,  0.0,  1.0
    );
    return Rz*Ry*Rx;
}


mat4 project ( GLfloat fovY , GLfloat ratio , GLfloat near , GLfloat far ) {
fprintf(stderr,"%s:%d:MARCO\n",__FILE__,__LINE__); //XXX
    const GLfloat scaleY  = cot((fovY*DEG2RAD)/2.0);
    const GLfloat scaleX  = scaleY/ratio;
    const GLfloat frustum = far-near;
fprintf(stderr,"%s:%d:POLO!\n",__FILE__,__LINE__); //XXX
    
    return mat4(
        scaleX,0,0,0,
        0,scaleY,0,0,
        0,0,-((far+near)/frustum),-((2*near*far)/frustum),
        0,0,-1,0
    );
}


//vec3 normalize ( vec3 v ) {
//    GLfloat m = sqrt((v.x*v.x)+(v.y*v.y)+(v.z*v.z));
//    return vec3(v.x/m,v.y/m,v.z/m);
//}


//mat4 look ( vec3 eye , vec3 center , vec3 up ) {
//    vec3 forward = normalize(center-eye);
//    vec3 side = mat3(
//        0,-forward.z,forward.y,
//        forward.z,0,
//    )*up;
//}


#endif
