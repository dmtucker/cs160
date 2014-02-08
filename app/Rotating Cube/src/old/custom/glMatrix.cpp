#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glew.h>

#include "glMatrix.h"

//#define _USE_MATH_DEFINES

double cot ( double rad ) {
    return tan(M_PI_2-rad);
}

double rad2deg ( double rad ) {
    return rad*(180/M_PI);
}

double deg2rad ( double deg ) {
    return deg*(M_PI/180);
}


/*
matrix identity ( ) {
    matrix I;
    for (int i = 0; i < 4 ;++i) {
        for ( int j = 0; j < 4 ;++j) {
            I.cell[i][j] = (i == j) ? 1 : 0;
        }
    }
    return I;
}
*/


matrix multiply ( const matrix * A , const matrix * B ) {
    matrix M = identity;
    for (int i = 0; i < 4 ;++i) {
        for (int j = 0; j < 4 ;++j) {
            M.cell[i][j] = (A->cell[i][0]*B->cell[0][j])+
                           (A->cell[i][1]*B->cell[1][j])+
                           (A->cell[i][2]*B->cell[2][j])+
                           (A->cell[i][3]*B->cell[3][j]);
        }
    }
    return M;
}


matrix project ( double FOVy , double aspectRatio , double near , double far ) {
    const double
        Y = cot(deg2rad(FOVy/2)),
        X = Y/aspectRatio,
        frustum = far-near;
    matrix P = {{{0}}};
    P.cell[0][0] = (GLfloat) X;
    P.cell[1][1] = (GLfloat) Y;
    P.cell[2][2] = (GLfloat) -((near+far)/frustum);
    P.cell[2][3] = (GLfloat) -1;
    P.cell[3][2] = (GLfloat) -((2*near*far)/frustum);
    return P;
}


void scale ( matrix * M , float x , float y , float z ) {
    matrix S = identity;
    S.cell[0][0] = x;
    S.cell[1][1] = y;
    S.cell[2][2] = z;
    S = multiply(M,&S);
    memcpy(M,&S.cell[0][0],sizeof(matrix));
}


void translate ( matrix * M , float x , float y , float z ) {
    matrix T = identity;
    T.cell[3][0] = x;
    T.cell[3][1] = y;
    T.cell[3][2] = z;
    T = multiply(M,&T);
    memcpy(M,&T.cell[0][0],sizeof(matrix));
}


void rotateX ( matrix * M , double rad ) {
    matrix R = identity;
    R.cell[1][1] = (GLfloat) cos(rad);
    R.cell[1][2] = (GLfloat) -sin(rad);
    R.cell[2][1] = (GLfloat) sin(rad);
    R.cell[2][2] = (GLfloat) cos(rad);
    R = multiply(M,&R);
    memcpy(M,&R.cell[0][0],sizeof(matrix));
}


void rotateY ( matrix * M , double rad ) {
    matrix R = identity;
    R.cell[0][0] = (GLfloat) cos(rad);
    R.cell[2][0] = (GLfloat) sin(rad);
    R.cell[0][2] = (GLfloat) -sin(rad);
    R.cell[2][2] = (GLfloat) cos(rad);
    R = multiply(M,&R);
    memcpy(M,&R.cell[0][0],sizeof(matrix));
}


void rotateZ ( matrix * M , double rad ) {
    matrix R = identity;
    R.cell[0][0] = (GLfloat) cos(rad);
    R.cell[0][1] = (GLfloat) -sin(rad);
    R.cell[1][0] = (GLfloat) sin(rad);
    R.cell[1][1] = (GLfloat) cos(rad);
    R = multiply(M,&R);
    memcpy(M,&R.cell[0][0],sizeof(matrix));
}


void print ( matrix * M ) {
    for (int i = 0; i < 4 ;++i) {
        for (int j = 0; j < 4 ;++j) {
            printf(" %d",(int) M->cell[i][j]);
        }
        putchar('\n');
    }
}
