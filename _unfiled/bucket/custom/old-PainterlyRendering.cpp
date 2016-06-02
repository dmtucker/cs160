// David Tucker <dmtucker@ucsc.edu>

//XXX
#define GREEK 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glMatrix.h"
#include "glExtras.h"

#define DEFAULT_WIDTH  800
#define DEFAULT_HEIGHT 600

char title[128] = "Sierpinski Gasket";
int window   = 0,   windowX = 0,   windowY = 0,   windowW = 0,   windowH = 0;
int windowed = 1, windowedX = 0, windowedY = 0, windowedW = 0, windowedH = 0;
int mouseX = 0, mouseY = 0;
unsigned int FPSrefreshRate = 1; // Hz
unsigned long int FPS = 0, populationSamples = 0, FSPS = 0, sampleFrames = 0;
GLuint shaderProgram, shaders[2], buffers[2], VAO;
matrix model,        view,        projection;
GLuint modelUniform, viewUniform, projectionUniform;


////////////////////////////////////////////////////////////////////////////////


void initShaders ( ) {
    shaders[0] = glShader("var/vertex.glsl",  GL_VERTEX_SHADER);
    shaders[1] = glShader("var/fragment.glsl",GL_FRAGMENT_SHADER);
    
    shaderProgram = glCreateProgram();
    glCheck("Shader Program Creation");
    glAttachShader(shaderProgram,shaders[0]);
    glAttachShader(shaderProgram,shaders[1]);
    glLinkProgram(shaderProgram);
    glCheck("Shader Program Linkage");
    
          viewUniform = glGetUniformLocation(shaderProgram,"view");
         modelUniform = glGetUniformLocation(shaderProgram,"model");
    projectionUniform = glGetUniformLocation(shaderProgram,"projection");
    glCheck("Shader Uniform Location");
}


void cleanShaders ( ) {
    glUseProgram(0);
    glDetachShader(shaderProgram,shaders[1]);
    glDetachShader(shaderProgram,shaders[0]);
    glDeleteProgram(shaderProgram);
    glDeleteShader(shaders[1]);
    glDeleteShader(shaders[0]);
    glCheck("Shader Destruction");
}



void initBuffers ( ) {
    glGenVertexArrays(1,&VAO);
    glCheck("VAO Generation");
    glBindVertexArray(VAO);
    glCheck("VAO Binding");
    
    //XXX
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glCheck("ERROR: Could not enable vertex attributes");
    
    glGenBuffers(2,&buffers[0]);
    glCheck("Buffer Generation");
    glBindBuffer(GL_ARRAY_BUFFER,buffers[0]);
    glCheck("VBO Binding");
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buffers[1]);
    glCheck("IBO Binding");
}


void cleanBuffers ( ) {
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glDeleteBuffers(2,&buffers[0]);
    glCheck("VBO Destruction");
    
    glBindVertexArray(0);
    glDeleteVertexArrays(1,&VAO);
    glCheck("VAO Destruction");
}


void adjusted ( ) {
    initShaders();
    initBuffers();
    translate(&view,0,0,-2);
    /*
    float size = 0.01f;
    const vertex vertices[4] = {
        {{Xn+size,Yn+size,0,1},{0,0,1,1}},
        {{Xn+size,Yn-size,0,1},{1,0,0,1}},
        {{Xn-size,Yn+size,0,1},{0,1,0,1}},
        {{Xn-size,Yn-size,0,1},{1,1,0,1}},
    };
    glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,sizeof(vertices),0);
    glVertexAttribPointer(
        1,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(vertices[0]),
        (GLvoid *) sizeof(vertices[0].XYZW)
    );
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBufferData(GL_ARRAY_BUFFER,sizeof(brush),brush,GL_STATIC_DRAW);
    */
    
    glBindVertexArray(VAO);
    
    const vertex VERTICES[8] = {
        { { -.5f, -.5f,  .5f, 1 }, { 0, 0, 1, 1 } },
        { { -.5f,  .5f,  .5f, 1 }, { 1, 0, 0, 1 } },
        { {  .5f,  .5f,  .5f, 1 }, { 0, 1, 0, 1 } },
        { {  .5f, -.5f,  .5f, 1 }, { 1, 1, 0, 1 } },
        { { -.5f, -.5f, -.5f, 1 }, { 1, 1, 1, 1 } },
        { { -.5f,  .5f, -.5f, 1 }, { 1, 0, 0, 1 } },
        { {  .5f,  .5f, -.5f, 1 }, { 1, 0, 1, 1 } },
        { {  .5f, -.5f, -.5f, 1 }, { 0, 0, 1, 1 } }
    };
    glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,sizeof(VERTICES),(GLvoid*) 0);
    glVertexAttribPointer(
        1,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(VERTICES),
        (GLvoid*) sizeof(VERTICES[0].XYZW)
    );
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBufferData(GL_ARRAY_BUFFER,sizeof(VERTICES),VERTICES,GL_STATIC_DRAW);
    const GLuint INDICES[36] = {
        0,2,1,  0,3,2,
        4,3,0,  4,7,3,
        4,1,5,  4,0,1,
        3,6,2,  3,7,6,
        1,6,5,  1,2,6,
        7,5,6,  7,4,5
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(INDICES),INDICES,GL_STATIC_DRAW);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    
    glBindVertexArray(0);
}


void bythebook ( ) {
    translate(&view,0,0,-2);
    
    const vertex VERTICES[8] = {
        { { -.5f, -.5f,  .5f, 1 }, { 0, 0, 1, 1 } },
        { { -.5f,  .5f,  .5f, 1 }, { 1, 0, 0, 1 } },
        { {  .5f,  .5f,  .5f, 1 }, { 0, 1, 0, 1 } },
        { {  .5f, -.5f,  .5f, 1 }, { 1, 1, 0, 1 } },
        { { -.5f, -.5f, -.5f, 1 }, { 1, 1, 1, 1 } },
        { { -.5f,  .5f, -.5f, 1 }, { 1, 0, 0, 1 } },
        { {  .5f,  .5f, -.5f, 1 }, { 1, 0, 1, 1 } },
        { {  .5f, -.5f, -.5f, 1 }, { 0, 0, 1, 1 } }
    };
    const GLuint INDICES[36] = {
        0,2,1,  0,3,2,
        4,3,0,  4,7,3,
        4,1,5,  4,0,1,
        3,6,2,  3,7,6,
        1,6,5,  1,2,6,
        7,5,6,  7,4,5
    };
    
    shaderProgram = glCreateProgram();
    glCheck("Shader Program Creation");
    shaders[0] = glShader("var/vertex.glsl",  GL_VERTEX_SHADER);
    shaders[1] = glShader("var/fragment.glsl",GL_FRAGMENT_SHADER);
    glAttachShader(shaderProgram,shaders[0]);
    glAttachShader(shaderProgram,shaders[1]);
    glLinkProgram(shaderProgram);
    glCheck("Shader Program Linkage");
    
          viewUniform = glGetUniformLocation(shaderProgram,"view");
         modelUniform = glGetUniformLocation(shaderProgram,"model");
    projectionUniform = glGetUniformLocation(shaderProgram,"projection");
    glCheck("Shader Uniform Location");
    
    glGenVertexArrays(1,&VAO);
    glCheck("VAO Generation");
    glBindVertexArray(VAO);
    glCheck("VAO Binding");
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glCheck("ERROR: Could not enable vertex attributes");
    
    glGenBuffers(2,&buffers[0]);
    glCheck("Buffer Generation");
    glBindBuffer(GL_ARRAY_BUFFER,buffers[0]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(VERTICES),VERTICES,GL_STATIC_DRAW);
    glCheck("VBO Binding");
    glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,sizeof(VERTICES),(GLvoid*) 0);
    glVertexAttribPointer(
        1,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(VERTICES),
        (GLvoid*) sizeof(VERTICES[0].XYZW)
    );
    glCheck("VAO Atrtribute Assignment");
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(INDICES),INDICES,GL_STATIC_DRAW);
    glCheck("IBO Binding");
    
    glBindVertexArray(0);
}


typedef struct {
    GLfloat x;
    GLfloat y;
} vec2;
typedef vec2 point2;
const unsigned int iterations = 5000;


void initDisplay ( ) {
if (GREEK) fprintf(stderr,"%s:%d:CHECKPOINT ZETA\n",__FILE__,__LINE__); //XXX
    //adjusted();
    //bythebook();
    
    vec2 points[iterations];
    vec2 
}


void cleanDisplay ( ) {
    cleanBuffers();
    cleanShaders();
if (GREEK) fprintf(stderr,"%s:%d:CHECKPOINT PHI\n",__FILE__,__LINE__); //XXX
}


void adjustedDraw ( ) {
    //float mouseXn = ((2*((float) mouseX))-((float) windowW))/((float) windowW);
    //float mouseYn = ((2*((float) mouseY))-((float) windowH))/((float) windowH);
    
    clock_t now = clock();
    if (then == 0) then = now;
    cubeRotation += 45.0f*((float) (now-then)/CLOCKS_PER_SEC);
    double cubeAngle = deg2rad(cubeRotation);
    then = now;
    
    model = identity;
    rotateY(&model,cubeAngle);
    rotateX(&model,cubeAngle);
    
    glUseProgram(shaderProgram);
    glCheck("Shader Program Assignment");
    
    glUniformMatrix4fv( viewUniform,1,GL_FALSE,& view.cell[0][0]);
    glUniformMatrix4fv(modelUniform,1,GL_FALSE,&model.cell[0][0]);
    glCheck("Shader Uniform Assignment");
    
    glBindVertexArray(VAO);
    glCheck("VAO Binding for Drawing");
    
    glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,(GLvoid *) 0);
    glCheck("Drawing");
    
    glBindVertexArray(0);
    glUseProgram(0);
}


void bythebookDraw ( ) {
    double cubeAngle;
    clock_t now = clock();

    if (then == 0)
        then = now;

    cubeRotation += 45.0f * ((float)(now - then) / CLOCKS_PER_SEC);
    cubeAngle = deg2rad(cubeRotation);
    then = now;

    model = identity;
    rotateY(&model, cubeAngle);
    rotateX(&model, cubeAngle);

    glUseProgram(shaderProgram);
    glCheck("ERROR: Could not use the shader program");

    glUniformMatrix4fv(modelUniform, 1, GL_FALSE, &model.cell[0][0]);
    glUniformMatrix4fv(viewUniform, 1, GL_FALSE, &view.cell[0][0]);
    glCheck("ERROR: Could not set the shader uniforms");

    glBindVertexArray(VAO);
    glCheck("ERROR: Could not bind the VAO for drawing purposes");

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLvoid*)0);
    glCheck("ERROR: Could not draw the cube");

    glBindVertexArray(0);
    glUseProgram(0);
}


void draw ( ) {
    //adjustedDraw();
    bythebookDraw();
}


////////////////////////////////////////////////////////////////////////////////


void render ( ) {
    ++sampleFrames;
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    sprintf(
        &title[0],
        "%dx%d | %dx%d | %ld FPS",
        mouseX,
        mouseY,
        windowW,
        windowH,
        FPS
    );
    glutSetWindowTitle(&title[0]);
    draw();
    
    glutSwapBuffers();
    glutPostRedisplay();
}


void resize ( int w , int h ) {
    windowW = w;
    windowH = h;
    glViewport(0,0,windowW,windowH);
    projection = project(60,(double) windowW/windowH,1,100);
    
    glUseProgram(shaderProgram);
    glCheck("Shader Program Assignment");
    glUniformMatrix4fv(projectionUniform,1,GL_FALSE,&projection.cell[0][0]);
    glCheck("Shader Uniform Assignment");
    glUseProgram(0);
}


void idle ( ) {
    glutPostRedisplay();
}


void update ( int x , int y ) {
    windowX = glutGet(GLUT_WINDOW_X);
    windowY = glutGet(GLUT_WINDOW_Y);
    windowW = glutGet(GLUT_WINDOW_WIDTH);
    windowH = glutGet(GLUT_WINDOW_HEIGHT);
    
    mouseX = x;
    mouseY = y;
}


void initFullscreen ( ) {
    windowedX = glutGet(GLUT_WINDOW_X);
    windowedY = glutGet(GLUT_WINDOW_Y);
    windowedW = glutGet(GLUT_WINDOW_WIDTH);
    windowedH = glutGet(GLUT_WINDOW_HEIGHT);
    glutFullScreen();
}


void cleanFullscreen ( ) {
    glutReshapeWindow(windowedW,windowedH);
    glutPositionWindow(windowedX,windowedY);
}


void keyboard ( unsigned char key , int x , int y ) {
    update(x,y);
    switch (key) {
        case 'F':
        case 'f':
            if (windowed) {
                windowed = 0;
                initFullscreen();
            }
            break;
        case 27: // escape
            if (!windowed) {
                windowed = 1;
                cleanFullscreen();
            }
            break;
        case 'Q':
        case 'q': glutLeaveMainLoop();
        default: break;
    }
}


void special ( int key , int x , int y ) {
    update(x,y);
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


void hover ( int x , int y ) {
    update(x,y);
}


void click ( int button , int state , int x , int y ) {
    update(x,y);
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
    update(x,y);
}


void timer ( int samples ) {
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
    glutTimerFunc(1000/FPSrefreshRate,timer,samples+1);
}


void cleanGL ( ) {
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
if (GREEK) fprintf(stderr,"%s:%d:CHECKPOINT CHI\n",__FILE__,__LINE__); //XXX
}


void cleanup ( ) {
    cleanDisplay();
    cleanGL();
if (GREEK) fprintf(stderr,"%s:%d:CHECKPOINT PSI\n",__FILE__,__LINE__); //XXX
}


void initGLUT ( int argc , char * argv[] ) {
if (GREEK) fprintf(stderr,"%s:%d:CHECKPOINT GAMMA\n",__FILE__,__LINE__); //XXX
    glutInit(&argc,argv);
    glutInitContextVersion(3,3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    //glutInitContextFlags(GLUT_FORWARD_COMPATIBLE); //XXX
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA); //XXX
    glutInitWindowSize(DEFAULT_WIDTH,DEFAULT_HEIGHT); //XXX
    
    glutSetOption(
        GLUT_ACTION_ON_WINDOW_CLOSE,
        GLUT_ACTION_GLUTMAINLOOP_RETURNS
    );
    
    window = glutCreateWindow(&title[0]);
    if (window < 1) {
        fprintf(stderr,"A new rendering window could not be created.\n");
        exit(EXIT_FAILURE);
    }
    
    glutDisplayFunc(render);
    glutReshapeFunc(resize);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutPassiveMotionFunc(hover);
    glutMouseFunc(click);
    glutMotionFunc(drag);
    glutTimerFunc(0,timer,0);
    glutCloseFunc(cleanup);
}


void initGLEW ( ) {
if (GREEK) fprintf(stderr,"%s:%d:CHECKPOINT DELTA\n",__FILE__,__LINE__); //XXX
    glewExperimental = GL_TRUE; //XXX : version < 4.0 only
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
if (GREEK) fprintf(stderr,"%s:%d:CHECKPOINT EPSILON\n",__FILE__,__LINE__); //XXX
    (void) glGetError();
    
    printf("OpenGL v%s\n",glGetString(GL_VERSION));
    
    glClearColor(0,0,0,0.5f);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glCheck("Depth Configuration");
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glCheck("Culling Configuration");
    
    view       = identity;
    model      = identity;
    projection = identity;
}


void initialize ( int argc , char * argv[] ) {
if (GREEK) fprintf(stderr,"%s:%d:CHECKPOINT BETA\n",__FILE__,__LINE__); //XXX
    initGLUT(argc,argv);
    initGLEW();
    //initGL(); //XXX
    initDisplay();
}


int main ( int argc , char * argv[] ) {
if (GREEK) fprintf(stderr,"%s:%d:CHECKPOINT ALPHA\n",__FILE__,__LINE__); //XXX
    initialize(argc,argv);
    glutMainLoop();
if (GREEK) fprintf(stderr,"%s:%d:CHECKPOINT OMEGA\n",__FILE__,__LINE__); //XXX
    return EXIT_SUCCESS;
}
