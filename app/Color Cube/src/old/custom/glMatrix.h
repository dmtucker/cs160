#ifndef GLMATRIX_H
#define GLMATRIX_H
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glew.h>

//#define _USE_MATH_DEFINES

typedef struct {
    GLfloat XYZW[4];
    GLfloat RGBA[4];
} vertex;
typedef struct {
    GLfloat cell[4][4];
} matrix;

static const matrix identity = {{
    {1,0,0,0},
    {0,1,0,0},
    {0,0,1,0},
    {0,0,0,1}
}};

double cot     ( double rad );
double rad2deg ( double rad );
double deg2rad ( double deg );

matrix multiply ( const matrix * A , const matrix * B );
matrix project  ( double FOVy , double aspectRatio , double near , double far );

void scale     ( matrix * M , float x , float y , float z );
void translate ( matrix * M , float x , float y , float z );
void rotateX   ( matrix * M , double rad );
void rotateY   ( matrix * M , double rad );
void rotateZ   ( matrix * M , double rad );
void print     ( matrix * M );
#endif
