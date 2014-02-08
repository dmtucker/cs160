// David Tucker <dmtucker@ucsc.edu>

//fprintf(stderr,"%s:%d:CHECKPOINT\n",__FILE__,__LINE__); //XXX
//fprintf(stderr,"%s:%d:CHECKPOINT MARCO\n",__FILE__,__LINE__); //XXX
//fprintf(stderr,"%s:%d:CHECKPOINT POLO!\n",__FILE__,__LINE__); //XXX


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Angel.h"
#include "shader.h"
#include "color.h"

#define DEFAULT_W 640
#define DEFAULT_H 480
#define MAX_FPS 32
#define WIREFRAME false

#define FPS (frames/glfwGetTime())

char title[128];
GLFWwindow * mainWindow = NULL;
int windowX = 0, windowY = 0, windowW = 0, windowH = 0;
double aspectRatio = DEFAULT_W/DEFAULT_H;
unsigned long int frames = 0;
double mouseX = 0, mouseY = 0;


////////////////////////////////////////////////////////////////////////////////


#include <math.h>
const unsigned int triangles = 12;
vec4 points[triangles*3], colors[triangles*3];
GLfloat Theta[3] = {0.0,0.0,0.0};
GLuint theta = 0;

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2
unsigned int axis = X_AXIS;


#define CUBE_S 0.5
//vec4 cube[8] = {
//    vec4(-CUBE_S,-CUBE_S,-CUBE_S,1.0), // left  lower back
//    vec4(-CUBE_S,-CUBE_S, CUBE_S,1.0), // left  lower front
//    vec4(-CUBE_S, CUBE_S,-CUBE_S,1.0), // left  upper back
//    vec4(-CUBE_S, CUBE_S, CUBE_S,1.0), // left  upper front
//    vec4( CUBE_S,-CUBE_S,-CUBE_S,1.0), // right lower back
//    vec4( CUBE_S,-CUBE_S, CUBE_S,1.0), // right lower front
//    vec4( CUBE_S, CUBE_S,-CUBE_S,1.0), // right upper back
//    vec4( CUBE_S, CUBE_S, CUBE_S,1.0)  // right upper front
//};
vec4 cube[8] = {
    vec4(-CUBE_S,-CUBE_S, CUBE_S,1.0), // left  lower front
    vec4(-CUBE_S, CUBE_S, CUBE_S,1.0), // left  upper front
    vec4( CUBE_S, CUBE_S, CUBE_S,1.0), // right upper front
    vec4( CUBE_S,-CUBE_S, CUBE_S,1.0), // right lower front
    vec4(-CUBE_S,-CUBE_S,-CUBE_S,1.0), // left  lower back
    vec4(-CUBE_S, CUBE_S,-CUBE_S,1.0), // left  upper back
    vec4( CUBE_S, CUBE_S,-CUBE_S,1.0), // right upper back
    vec4( CUBE_S,-CUBE_S,-CUBE_S,1.0)  // right lower back
};


//vec4 colorCubeColors[8] = {
//    palette[BASIC_BLACK],
//    palette[BASIC_BLUE],
//    palette[BASIC_GREEN],
//    palette[BASIC_CYAN],
//    palette[BASIC_RED],
//    palette[BASIC_MAGENTA],
//    palette[BASIC_YELLOW],
//    palette[BASIC_WHITE]
//};
vec4 colorCubeColors[8] = {
    palette[BASIC_BLACK],
    palette[BASIC_RED],
    palette[BASIC_YELLOW],
    palette[BASIC_GREEN],
    palette[BASIC_BLUE],
    palette[BASIC_MAGENTA],
    palette[BASIC_WHITE],
    palette[BASIC_CYAN]
};


void initQuad ( const unsigned int a , const unsigned int b , const unsigned int c , const unsigned int d ) {
    static unsigned int colorIndex = 0;
    
    points[colorIndex] = cube[a];
    colors[colorIndex] = colorCubeColors[a];
    ++colorIndex;
    
    points[colorIndex] = cube[b];
    colors[colorIndex] = colorCubeColors[b];
    ++colorIndex;
    
    points[colorIndex] = cube[c];
    colors[colorIndex] = colorCubeColors[c];
    ++colorIndex;
    
    points[colorIndex] = cube[a];
    colors[colorIndex] = colorCubeColors[a];
    ++colorIndex;
    
    points[colorIndex] = cube[c];
    colors[colorIndex] = colorCubeColors[c];
    ++colorIndex;
    
    points[colorIndex] = cube[d];
    colors[colorIndex] = colorCubeColors[d];
    ++colorIndex;
}


void colorCube ( ) {
//    initQuad(0,2,6,4); // back
//    initQuad(1,0,4,5); // bottom
//    initQuad(2,0,1,3); // left
//    initQuad(3,7,6,2); // top
//    initQuad(4,6,7,5); // right
//    initQuad(5,7,3,1); // front
    initQuad(1,0,3,2); // front
    initQuad(2,3,7,6); // right
    initQuad(3,0,4,7); // bottom
    initQuad(6,5,1,2); // top
    initQuad(4,5,6,7); // back
    initQuad(5,4,0,1); // left
}


void initFrame ( ) {
    colorCube();
}


void initDisplay ( ) {
    initFrame();
//    for (int i = 0; i < triangles*3 ;++i) {
//        printf("(%f,%f,%f)",points[i][0],points[i][1],points[i][2]);
//             if (colors[i][0] == 1.0 and colors[i][1] == 0.0) puts("red");
//        else if (colors[i][1] == 1.0 and colors[i][2] == 0.0) puts("green");
//        else if (colors[i][2] == 1.0 and colors[i][0] == 0.0) puts("blue");
//        else                                                  puts("white");
//    }
    
    GLuint program = initShaders("src/vertex.glsl","src/fragment.glsl");
    glUseProgram(program);
    checkGL(__FILE__,__LINE__);
    
    GLuint vao;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    checkGL(__FILE__,__LINE__);
    
    GLuint buffer;
    glGenBuffers(1,&buffer);
    glBindBuffer(GL_ARRAY_BUFFER,buffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(points)+sizeof(colors),NULL,GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER,0             ,sizeof(points),points);
    glBufferSubData(GL_ARRAY_BUFFER,sizeof(points),sizeof(colors),colors);
    checkGL(__FILE__,__LINE__);
    
    GLuint loc1 = glGetAttribLocation(program,"vPosition");
    glEnableVertexAttribArray(loc1);
    glVertexAttribPointer(loc1,4,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(0));
    checkGL(__FILE__,__LINE__);
    
    GLuint loc2 = glGetAttribLocation(program,"vColor");
    glEnableVertexAttribArray(loc2);
    glVertexAttribPointer(loc2,4,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(sizeof(points)));
    checkGL(__FILE__,__LINE__);
    
    theta = glGetUniformLocation(program,"theta");
}


void idleFrame ( ) {
    Theta[axis] += 0.05;
    if(Theta[axis] > 360) Theta[axis] -= 360;
}


void drawFrame ( ) {
    idleFrame();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glUniform3fv(theta,1,Theta);
    glDrawArrays(GL_TRIANGLES,0,triangles*3);
    checkGL(__FILE__,__LINE__);
}


void cleanDisplay ( ) {
    //XXX
}


////////////////////////////////////////////////////////////////////////////////


static void error ( int error , const char * description ) {
    fputs(description,stderr);
}


static void resize ( GLFWwindow * window , int width , int height ) {
    glViewport(0,0,width,height);
    checkGL(__FILE__,__LINE__);
    aspectRatio = width/(double) height;
}


static void keyboard ( GLFWwindow * window , int key , int scancode , int action , int mods ) {
    switch (action) {
        case GLFW_PRESS: break;
        default:
            fprintf(
                stderr,
                "A key action could not be recognized (%d).\n",
                action
            );
        case GLFW_RELEASE:
        case GLFW_REPEAT: return;
    }
    switch (key) {
        // http://www.glfw.org/docs/latest/group__keys.html
        // http://www.glfw.org/docs/latest/group__mods.html
        case GLFW_KEY_Q: glfwSetWindowShouldClose(window,GL_TRUE);
        default: break;
    }
}


static void mouse ( GLFWwindow * window , int button , int action , int mods ) {
    switch (action) {
        default:
            fprintf(
                stderr,
                "A button action could not be recognized (%d).\n",
                action
            );
        case GLFW_PRESS:   break;
        case GLFW_RELEASE: return;
    }
    switch (button) {
        // http://www.glfw.org/docs/latest/group__buttons.html
        // http://www.glfw.org/docs/latest/group__mods.html
        case GLFW_MOUSE_BUTTON_LEFT:   axis = X_AXIS; break;
        case GLFW_MOUSE_BUTTON_MIDDLE: axis = Y_AXIS; break;
        case GLFW_MOUSE_BUTTON_RIGHT:  axis = Z_AXIS; break;
        default: break;
    }
}


static void cursor ( GLFWwindow * window , double xpos , double ypos ) {
    mouseX = xpos;
    mouseY = ypos;
}


static void scroll ( GLFWwindow * window , double xoffset , double yoffset ) {
    //XXX
}


////////////////////////////////////////////////////////////////////////////////


void initGLFW ( ) {
    glfwSetErrorCallback(error);
    if (not glfwInit()) {
        fputs("GLFW Initialization Error",stderr);
        exit(EXIT_FAILURE);
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,GL_TRUE); //XXX
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    mainWindow = glfwCreateWindow(DEFAULT_W,DEFAULT_H,&title[0],NULL,NULL);
    if (mainWindow == NULL) {
        glfwTerminate();
        fputs("Window Creation Error",stderr);
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(mainWindow);

    //glfwSetWindowSizeCallback(mainWindow,resize); // screen coordinates
    glfwSetFramebufferSizeCallback(mainWindow,resize); // pixels
    glfwSetKeyCallback(mainWindow,keyboard);
    glfwSetMouseButtonCallback(mainWindow,mouse);
    glfwSetCursorPosCallback(mainWindow,cursor);
    glfwSetScrollCallback(mainWindow,scroll);
    
    printf("GLFW v%s\n",glfwGetVersionString());
}


void initGLEW ( ) {
    glewExperimental = GL_TRUE; //XXX < v4.0
    GLenum status = glewInit();
    if (status != GLEW_OK) {
        fprintf(
            stderr,
            "GLEW Initialization Error (%s).\n",
            glewGetErrorString(status)
        );
        exit(EXIT_FAILURE);
    }
    
    printf("GLEW v%s\n",glewGetString(GLEW_VERSION));
}


void initGL ( ) {
    (void) glGetError();
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    checkGL(__FILE__,__LINE__);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    checkGL(__FILE__,__LINE__);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    checkGL(__FILE__,__LINE__);
    
    if (WIREFRAME) {
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        checkGL(__FILE__,__LINE__);
    }
    
    glClearColor(0.0,0.0,0.0,1.0);
    
    printf("OpenGL v%s\n",glGetString(GL_VERSION));
}


void initialize () {
    initGLFW();
    initGLEW();
    initGL();
    initDisplay();
}


void meta ( ) {
    glfwGetWindowPos (mainWindow,&windowX,&windowY);
    glfwGetWindowSize(mainWindow,&windowW,&windowH);
    sprintf(
        &title[0],
        "%ld FPS | %dx%d (%d,%d)",
        (int) round(FPS),
        windowW,windowH,
        (int) mouseX, (int) mouseY
    );
}


void cleanGL ( ) {
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}


void cleanGLFW ( ) {
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}


void cleanup ( ) {
    cleanDisplay();
    cleanGL();
    cleanGLFW();
}


int main ( int argc , char * argv[] ) {
    initialize();
    while (not glfwWindowShouldClose(mainWindow)) {
        meta();
        if (FPS > MAX_FPS) idleFrame();
        else {
            drawFrame();
            glfwSwapBuffers(mainWindow);
            glfwSetWindowTitle(mainWindow,&title[0]);
            ++frames;
        }
        
        //glfwWaitEvents();
        glfwPollEvents();
    }
    cleanup();
    return EXIT_SUCCESS;
}
