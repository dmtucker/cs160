#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
//angle of rotation
float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0, angle=0.0;

float lastx, lasty;

//positions of the cubes
float positionz[10];
float positionx[10];



void camera (void) {
    glRotatef(xrot,1.0,0.0,0.0);  //rotate our camera on teh x-axis (left and right)
    glRotatef(yrot,0.0,1.0,0.0);  //rotate our camera on the y-axis (up and down)
    glTranslated(-xpos,-ypos,-zpos); //translate the screen to the position of our camera
}

void display (void) {
    glLoadIdentity();  
    camera();
    angle++; //increase the angle
}

void keyboard (unsigned char key, int x, int y) {
    if (key==‘q’)
    {
    xrot += 1;
    if (xrot >360) xrot -= 360;
    }

    if (key==‘z’)
    {
    xrot -= 1;
    if (xrot < -360) xrot += 360;
    }

    if (key==‘w’)
    {
    float xrotrad, yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xrotrad = (xrot / 180 * 3.141592654f); 
    xpos += float(sin(yrotrad)) ;
    zpos -= float(cos(yrotrad)) ;
    ypos -= float(sin(xrotrad)) ;
    }

    if (key==‘s’)
    {
    float xrotrad, yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xrotrad = (xrot / 180 * 3.141592654f); 
    xpos -= float(sin(yrotrad));
    zpos += float(cos(yrotrad)) ;
    ypos += float(sin(xrotrad));
    }

    if (key==‘d’)
    {
    float yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xpos += float(cos(yrotrad)) * 0.2;
    zpos += float(sin(yrotrad)) * 0.2;
    }

    if (key==‘a’)
    {
    float yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xpos -= float(cos(yrotrad)) * 0.2;
    zpos -= float(sin(yrotrad)) * 0.2;
    }

    if (key==27)
    {
    exit(0);
    }
}

void mouseMovement(int x, int y) {
    int diffx=x-lastx; //check the difference between the current x and the last x position
    int diffy=y-lasty; //check the difference between the current y and the last y position
    lastx=x; //set lastx to the current x position
    lasty=y; //set lasty to the current y position
    xrot += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position
    yrot += (float) diffx;    //set the xrot to yrot with the addition of the difference in the x position
}

int main (int argc, char **argv) {
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH); 
    glutInitWindowSize (500, 500); 
    glutInitWindowPosition (100, 100);
    glutCreateWindow (“A basic OpenGL Window“); 
    init (); 
    glutDisplayFunc (display); 
    glutIdleFunc (display); 
    glutReshapeFunc (reshape);

    glutPassiveMotionFunc(mouseMovement); //check for mouse
 movement

    glutKeyboardFunc (keyboard); 
    glutMainLoop (); 
    return 0;
}
