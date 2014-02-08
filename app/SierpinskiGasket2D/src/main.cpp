// David Tucker <dmtucker@ucsc.edu>

#define GREEK 0

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Angel.h"

#define DEFAULT_WIDTH  800
#define DEFAULT_HEIGHT 800

const unsigned int FPSrefreshRate = 1, FPSlimit = 60; // Hz
const unsigned int iterations = 7, triangles  = 6561;
vec2 points[3*triangles];

void triangle ( const vec2& a , const vec2& b , const vec2& c ) {
    static unsigned int i = 0;
    
    points[i++] = a;
    points[i++] = b;
    points[i++] = c;
}

void divideTriangle ( const vec2& a , const vec2& b , const vec2& c , int count ) {
    if (count > 0) {
        vec2 v0 = (a+b)/2.0;
        vec2 v1 = (a+c)/2.0;
        vec2 v2 = (b+c)/2.0;
        divideTriangle(a,v0,v1,count-1);
        divideTriangle(c,v1,v2,count-1);
        divideTriangle(b,v2,v0,count-1);
    }
    else triangle(a,b,c);
}

////////////////////////////////////////////////////////////////////////////////

char title[128];
int window = 0, windowX = 0, windowY = 0, windowW = 0, windowH = 0;
bool fullScreen = false;
unsigned long int FPS = 0, sampleFrames = 0;
int mouseX = 0, mouseY = 0;


void updateWindow ( ) {
    windowX = glutGet(GLUT_WINDOW_X);
    windowY = glutGet(GLUT_WINDOW_Y);
    windowW = glutGet(GLUT_WINDOW_WIDTH);
    windowH = glutGet(GLUT_WINDOW_HEIGHT);
}


void resize ( int w , int h ) {
    glViewport(0,0,w,h);
    updateWindow();
}


bool toggleFullscreen ( bool enable = !fullScreen ) {
    static int windowedX = 0;
    static int windowedY = 0;
    static int windowedW = 0;
    static int windowedH = 0;
    
    if (enable) {
        windowedX = glutGet(GLUT_WINDOW_X);
        windowedY = glutGet(GLUT_WINDOW_Y);
        windowedW = glutGet(GLUT_WINDOW_WIDTH);
        windowedH = glutGet(GLUT_WINDOW_HEIGHT);
        glutFullScreen();
        fullScreen = true;
    }
    else {
        glutReshapeWindow(windowedW,windowedH);
        //XXX glutPositionWindow(windowedX,windowedY);
        fullScreen = false;
    }
    return fullScreen;
}


bool limitFPS ( ) {
    static clock_t last = 0;
    clock_t now = clock()/(CLOCKS_PER_SEC/1000);
    clock_t elapsed = now-last;
    if (elapsed < 1000/FPSlimit) return true;
    last = now;
    return false;
}


void drawTitle ( ) {
    sprintf(
        &title[0],
        "(%d,%d) | %ld FPS | (%d,%d) %dx%d",
        mouseX,mouseY,
        FPS,
        windowX,windowY,
        windowW,windowH
    );
    glutSetWindowTitle(&title[0]);
}


void drawFrame ( ) {
    glDrawArrays(GL_TRIANGLES,0,triangles);
}


void display ( ) {
    if (limitFPS()) return;
    ++sampleFrames;
    glClear(GL_COLOR_BUFFER_BIT); //XXX |GL_DEPTH_BUFFER_BIT);
    drawTitle();
    drawFrame();
    glFlush();
    //XXX glutSwapBuffers();
    //XXX glutPostRedisplay();
}


void updateFPS ( int samples ) {
    static unsigned long int populationSamples = 0, FSPS = 0;
    
    if (samples > 0) {
        if (samples < (int) (10*FPSrefreshRate)) {
            FSPS += sampleFrames;
        }
        else {
            if (populationSamples+1 < populationSamples) populationSamples = 1;
            else ++populationSamples;
            unsigned long populationSampleFPS = (FSPS*FPSrefreshRate)/samples;
            populationSampleFPS /= populationSamples;
            FPS += populationSampleFPS;
            FPS *= populationSamples/(populationSamples+1);
            FSPS = samples = 0;
        }
        if (FPS == 0 || populationSamples == 0) {
            FPS = (((FSPS == 0) ? sampleFrames : FSPS/samples))*FPSrefreshRate;
        }
        sampleFrames = 0;
    }
    glutTimerFunc(1000/FPSrefreshRate,updateFPS,samples+1);
}


void updateMouse ( int x , int y ) {
    mouseX = x;
    mouseY = y;
}


void hover ( int x , int y ) {
    updateMouse(x,y);
}


void click ( int button , int state , int x , int y ) {
    updateMouse(x,y);
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_UP) break;
            else break;
            break;
        case GLUT_MIDDLE_BUTTON:
            if (state == GLUT_UP) break;
            else break;
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_UP) break;
            else break;
            break;
        default:
            fprintf(
                stderr,
                "A mouse button could not be recognized (%d).\n",
                button
            );
            break;
    }
}


void drag ( int x , int y ) {
    updateMouse(x,y);
}


void keyboard ( unsigned char key , int x , int y ) {
    updateMouse(x,y);
    switch (key) {
        case 'F': if (fullScreen) break;
        case 'f': toggleFullscreen(); break;
        case 27: // escape
            if (fullScreen) toggleFullscreen();
            break;
        case 'Q':
        case 'q': glutLeaveMainLoop();
        default: break;
    }
}


void special ( int key , int x , int y ) {
    updateMouse(x,y);
    switch (key) {
        case GLUT_KEY_F1:        break;
        case GLUT_KEY_F2:        break;
        case GLUT_KEY_F3:        break;
        case GLUT_KEY_F4:        break;
        case GLUT_KEY_F5:        break;
        case GLUT_KEY_F6:        break;
        case GLUT_KEY_F7:        break;
        case GLUT_KEY_F8:        break;
        case GLUT_KEY_F9:        break;
        case GLUT_KEY_F10:       break;
        case GLUT_KEY_F11:       break;
        case GLUT_KEY_F12:       break;
        case GLUT_KEY_LEFT:      break;
        case GLUT_KEY_UP:        break;
        case GLUT_KEY_RIGHT:     break;
        case GLUT_KEY_DOWN:      break;
        case GLUT_KEY_PAGE_UP:   break;
        case GLUT_KEY_PAGE_DOWN: break;
        case GLUT_KEY_HOME:      break;
        case GLUT_KEY_END:       break;
        case GLUT_KEY_INSERT:    break;
        default:
            fprintf(
                stderr,
                "A special keyboard key could not be recognized (%d).\n",
                key
            );
            break;
    }
}


void idle ( ) {
    updateWindow();
    glutPostRedisplay();
}


void cleanDisplay ( ) {
    //XXX
}


void cleanGL ( ) {
    //XXX
//    glDisable(GL_CULL_FACE);
//    glDisable(GL_DEPTH_TEST);
}


void cleanup ( ) {
    cleanDisplay();
    cleanGL();
}


void initGLUT ( int argc , char * argv[] ) {
    glutInit(&argc,argv);
    glutInitContextVersion(3,3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    //XXX glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitDisplayMode(GLUT_RGBA); //XXX |GLUT_DEPTH|GLUT_DOUBLE);
    glutInitWindowSize(DEFAULT_WIDTH,DEFAULT_HEIGHT);
    
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    
    window = glutCreateWindow(&title[0]);
    if (window < 1) {
        fprintf(stderr,"A new rendering window could not be created.\n");
        exit(EXIT_FAILURE);
    }
    
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutTimerFunc(0,updateFPS,0);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutPassiveMotionFunc(hover);
    glutMouseFunc(click);
    glutMotionFunc(drag);
    glutIdleFunc(idle);
    glutCloseFunc(cleanup);
}


void initGLEW ( ) {
    glewExperimental = GL_TRUE; //XXX < v4.0
    GLenum status = glewInit();
    if (status != GLEW_OK) {
        fprintf(
            stderr,
            "GLEW could not be initialized (%s).\n",
            glewGetErrorString(status)
        );
        exit(EXIT_FAILURE);
    }
}


void initGL ( ) {
    (void) glGetError();
    
    printf("OpenGL v%s\n",glGetString(GL_VERSION));
    
    //XXX
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LESS);
//    glCheck("Depth Configuration");
//    
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
//    glFrontFace(GL_CCW);
//    glCheck("Culling Configuration");
}


void initDisplay ( ) {
//    vec2 points[iterations];
//    vec2 vertices[3] = {vec2(-1.0,-1.0),vec2(0.0,1.0),vec2(1.0,-1.0)};
//    points[0] = vec2(0.25,0.50);
//    for (int i = 1; i < iterations ;++i) {
//        int j = rand()%3;
//        points[i] = (points[i-1]+vertices[j])/2.0;
//    }
    divideTriangle(vec2(-1.0,-1.0),vec2(0.0,1.0),vec2(1.0,-1.0),iterations);
    
    GLuint program = InitShader("src/vertex.glsl","src/fragment.glsl");
    glUseProgram(program);
    
    GLuint vao;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    
    GLuint buffer;
    glGenBuffers(1,&buffer);
    glBindBuffer(GL_ARRAY_BUFFER,buffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(points),points,GL_STATIC_DRAW);
    
    GLuint loc = glGetAttribLocation(program,"vPosition");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc,2,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(0));
    
    glClearColor(0.0,0.0,0.0,1.0);
}


void initialize ( int argc , char * argv[] ) {
    initGLUT(argc,argv);
    initGLEW();
    initGL();
    initDisplay();
}


int main ( int argc , char * argv[] ) {
if (GREEK) fprintf(stderr,"%s:%d:CHECKPOINT ALPHA\n",__FILE__,__LINE__); //XXX
    initialize(argc,argv);
if (GREEK) fprintf(stderr,"%s:%d:CHECKPOINT BETA\n",__FILE__,__LINE__); //XXX
    glutMainLoop();
if (GREEK) fprintf(stderr,"%s:%d:CHECKPOINT OMEGA\n",__FILE__,__LINE__); //XXX
    return EXIT_SUCCESS;
}
