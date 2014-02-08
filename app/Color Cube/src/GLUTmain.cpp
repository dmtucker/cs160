// David Tucker <dmtucker@ucsc.edu>


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "shader.h"
#include "Angel.h"

#define DEFAULT_X 0
#define DEFAULT_Y 0
#define DEFAULT_W 640
#define DEFAULT_H 480
#define MAX_FPS 60 // Hz
#define FPS_REFRESH 1 // Hz

char title[128];
int window = 0, windowX = 0, windowY = 0, windowW = 0, windowH = 0;
double aspectRatio = DEFAULT_W/DEFAULT_H, FPS = MAX_FPS;
double mouseX = 0, mouseY = 0;
bool fullScreen = false;
unsigned long int sampleFrames = 0;


////////////////////////////////////////////////////////////////////////////////


#include <math.h>
const unsigned int iterations = 1, triangles = pow(4,iterations+1);
vec4 points[triangles*3], colors[triangles*3];
vec4 baseColors[4] = {
    vec4(0.0,0.0,0.0,1.0),
    vec4(1.0,0.0,0.0,1.0),
    vec4(0.0,1.0,0.0,1.0),
    vec4(0.0,0.0,1.0,1.0)
};
unsigned int baseColor = 0;


void triangle ( const vec4 a , const vec4 b , const vec4 c ) {
    static unsigned int i = 0;
    
    points[i] = a;
    colors[i] = baseColors[baseColor];
    ++i;
    
    points[i] = b;
    colors[i] = baseColors[baseColor];
    ++i;
    
    points[i] = c;
    colors[i] = baseColors[baseColor];
    ++i;
}


void drawTetrahedron ( const vec4 a , const vec4 b , const vec4 c , const vec4 d ) {
    baseColor = 0;
    triangle(a,b,c);
    baseColor = 1;
    triangle(a,c,d);
    baseColor = 2;
    triangle(a,d,b);
    baseColor = 3;
    triangle(b,d,c);
}


void initTetrahedron ( const vec4 a , const vec4 b , const vec4 c , const vec4 d , int count ) {
    if (count > 0) {
        vec4 m[6];
        m[0] = (a+b)/2.0;
        m[1] = (a+c)/2.0;
        m[2] = (a+d)/2.0;
        m[3] = (b+c)/2.0;
        m[4] = (b+d)/2.0;
        m[5] = (c+d)/2.0;
        initTetrahedron(a   ,m[0],m[1],m[2],count-1);
        initTetrahedron(m[0],b   ,m[3],m[4],count-1);
        initTetrahedron(m[1],m[3],c   ,m[5],count-1);
        initTetrahedron(m[2],m[4],m[5],d   ,count-1);
    }
    else drawTetrahedron(a,b,c,d);
}


void initDisplay ( ) {
    initTetrahedron(
        vec4(-1.0,-1.0,-1.0,0.0),
        vec4( 1.0,-1.0,-1.0,0.0),
        vec4( 0.0, 1.0,-1.0,0.0),
        vec4( 0.0, 0.0, 0.0,0.0),
        iterations
    );
    
//    for (int i = 0; i < triangles*3 ;++i) {
//        printf("(%f,%f,%f)",points[i][0],points[i][1],points[i][2]);
//             if (colors[i][0] == 1.0 and colors[i][1] == 0.0) puts("red");
//        else if (colors[i][1] == 1.0 and colors[i][2] == 0.0) puts("green");
//        else if (colors[i][2] == 1.0 and colors[i][0] == 0.0) puts("blue");
//        else                                                  puts("white");
//    }
    
    GLuint program = initShaders("src/vertex.glsl","src/fragment.glsl");
    glUseProgram(program);
    
    GLuint vao;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    
    GLuint buffer;
    glGenBuffers(1,&buffer);
    glBindBuffer(GL_ARRAY_BUFFER,buffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(points)+sizeof(colors),NULL,GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(points),points);
    glBufferSubData(GL_ARRAY_BUFFER,sizeof(points),sizeof(colors),colors);
    
    GLuint loc1 = glGetAttribLocation(program,"vPosition");
    glEnableVertexAttribArray(loc1);
    glVertexAttribPointer(loc1,4,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(0));
    
    GLuint loc2 = glGetAttribLocation(program,"vColor");
    glEnableVertexAttribArray(loc2);
    glVertexAttribPointer(loc2,4,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(sizeof(points)));
}


void drawFrame ( ) {
    glDrawArrays(GL_TRIANGLES,0,triangles*3);
}


//void drawFrame ( ) {
//    float ratio;
//    int width, height;
//    glfwGetFramebufferSize(mainWindow, &width, &height);
//    ratio = width / (float) height;
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
//    glBegin(GL_TRIANGLES);
//    glColor3f(1.f, 0.f, 0.f);
//    glVertex3f(-0.6f, -0.4f, 0.f);
//    glColor3f(0.f, 1.f, 0.f);
//    glVertex3f(0.6f, -0.4f, 0.f);
//    glColor3f(0.f, 0.f, 1.f);
//    glVertex3f(0.f, 0.6f, 0.f);
//    glEnd();
//}


void cleanDisplay ( ) {
    //XXX
}


////////////////////////////////////////////////////////////////////////////////


void updateFPS ( int samples ) {
    static unsigned long int populationSamples = 0, FSPS = 0;
    
    if (samples > 0) {
        if (samples < (int) (10*FPS_REFRESH)) {
            FSPS += sampleFrames;
        }
        else {
            if (populationSamples+1 < populationSamples) populationSamples = 1;
            else ++populationSamples;
            unsigned long populationSampleFPS = (FSPS*FPS_REFRESH)/samples;
            populationSampleFPS /= populationSamples;
            FPS += populationSampleFPS;
            FPS *= populationSamples/(populationSamples+1);
            FSPS = samples = 0;
        }
        if (FPS == 0 || populationSamples == 0) {
            FPS = (((FSPS == 0) ? sampleFrames : FSPS/samples))*FPS_REFRESH;
        }
        sampleFrames = 0;
    }
    glutTimerFunc(1000/FPS_REFRESH,updateFPS,samples+1);
}


void resize ( int width , int height ) {
    glViewport(0,0,width,height);
    aspectRatio = width/(double) height;
}


bool toggleFullscreen ( bool enable = !fullScreen ) {
    if (enable) {
        glutFullScreen();
        fullScreen = true;
    }
    else {
        //XXX glutLeaveFullScreen();
        glutReshapeWindow(DEFAULT_W,DEFAULT_H);
        fullScreen = false;
    }
    return fullScreen;
}


void keyboard ( unsigned char key , int x , int y ) {
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


void mouse ( int button , int state , int x , int y ) {
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
    //XXX
}


void hover ( int x , int y ) {
    mouseX = x;
    mouseY = y;
}


bool limitFPS ( ) {
    static clock_t last = 0;
    clock_t now = clock()/(CLOCKS_PER_SEC/1000);
    clock_t elapsed = now-last;
    if (elapsed < 1000/MAX_FPS) return true;
    last = now;
    return false;
}


void drawTitle ( ) {
    sprintf(
        &title[0],
        "%ld FPS | %dx%d (%d,%d)",
        (int) FPS,
        windowW,windowH,
        (int) mouseX, (int) mouseY
    );
    glutSetWindowTitle(&title[0]);
}


void display ( ) {
    if (limitFPS()) return;
    ++sampleFrames;
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    drawTitle();
    drawFrame();
    glutSwapBuffers();
}


void idle ( ) {
    windowX = glutGet(GLUT_WINDOW_X);
    windowY = glutGet(GLUT_WINDOW_Y);
    windowW = glutGet(GLUT_WINDOW_WIDTH);
    windowH = glutGet(GLUT_WINDOW_HEIGHT);
    glutPostRedisplay();
}


void cleanGL ( ) {
    //XXX
//    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}


void cleanup ( ) {
    cleanDisplay();
    cleanGL();
}


void initGLUT ( int argc , char * argv[] ) {
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
    glutInitWindowPosition(DEFAULT_X,DEFAULT_Y);
    glutInitWindowSize(DEFAULT_W,DEFAULT_H);
    
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    
    window = glutCreateWindow(&title[0]);
    if (window < 1) {
        fprintf(stderr,"A new rendering window could not be created.\n");
        exit(EXIT_FAILURE);
    }
    glutInitContextVersion(3,3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    
    glutDisplayFunc(display);
    glutTimerFunc(0,updateFPS,0);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMouseFunc(mouse);
    glutMotionFunc(drag);
    glutPassiveMotionFunc(hover);
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
    
    glClearColor(0.0,0.0,0.0,1.0);
    
    //XXX
    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LESS);
//    glCheck("Depth Configuration");
//    
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
//    glFrontFace(GL_CCW);
//    glCheck("Culling Configuration");
}


void initialize ( int argc , char * argv[] ) {
    initGLUT(argc,argv);
    initGLEW();
    initGL();
    initDisplay();
}


int main ( int argc , char * argv[] ) {
    initialize(argc,argv);
    glutMainLoop();
    return EXIT_SUCCESS;
}


//fprintf(stderr,"%s:%d:CHECKPOINT\n",__FILE__,__LINE__); //XXX
//fprintf(stderr,"%s:%d:CHECKPOINT MARCO\n",__FILE__,__LINE__); //XXX
//fprintf(stderr,"%s:%d:CHECKPOINT POLO!\n",__FILE__,__LINE__); //XXX
