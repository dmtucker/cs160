// Lab3.cpp : Terrain Mapping
//

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>

#include "ppm_canvas.h"
#include "util.h"

#define PI 3.14159265

float xrot, yrot = 0;
bool keyState[256] = { false };
int windowWidth, windowHeight;

canvas_t terrain;
canvas_t skin;
GLuint texture;

float lightPosition[4];
float diffuseColour[3];
float ambientColour[3];

void drawTexture() {
	int x, z;
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	gluLookAt(0,0,15,0,0,0,0,1,0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f);
		glNormal3f(0,0.0,1.0);
		glVertex3f(-10,-10,0);

		glTexCoord2f(0.0f,1.0f);
		glVertex3f(-10,10,0);

		glTexCoord2f(1.0f,1.0f);
		glVertex3f(10,10,0);

		glTexCoord2f(1.0f,0.0f);
		glVertex3f(10,-10,0);
	glEnd();
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glDisable(GL_TEXTURE_2D);
}


void cb_display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	draw_axis(4.0);
	drawTexture();
	glFlush();
	glutSwapBuffers(); // for smoother animation
}

void loadTextureMap(canvas_t tex) {
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tex.height, tex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.pixels);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
}

void cb_idle() {	
	drawTexture();
	glutPostRedisplay();
}

void cb_reshape(int w, int h) {
	windowWidth = w;
	windowHeight = h;
	int aspect = w/h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, aspect, 0.1, 200); // necessary to preserve aspect ratio
	glMatrixMode(GL_MODELVIEW);
}

void cb_mouseclick(int button, int state, int x, int y) {
}

void cb_mousemove(int x, int y) {
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Lab 3: Terrain Mapping");
	glutIgnoreKeyRepeat(true);

	glEnable(GL_TEXTURE_2D);
	if (argc == 3) {
		canvas_t height;
		printf("Loading file '%s'... ", argv[1]);
		ppmLoadCanvas(argv[1], &height);
		printf("Done.\n");
		canvas_t texture;
		printf("Loading file '%s'... ", argv[2]);
		ppmLoadCanvas(argv[2], &texture);
		loadTextureMap(texture);
		
	} else {
		printf("Usage: %s terrain.ppm texture.ppm\n", argv[0]);
		return 1;
	}

	//terrain->drawTerrain();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glEnable (GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	lightPosition[0] = 0;
	lightPosition[1] = 0;
	lightPosition[2] = 50;
	
	glutDisplayFunc(cb_display);
	glutReshapeFunc(cb_reshape);
	glutIdleFunc(cb_idle);

	glClearColor(0,0,0,0); // set background to black

	glutMainLoop();

	return 0;
}
