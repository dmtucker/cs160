// David Tucker <dmtucker@ucsc.edu>

//fprintf(stderr,"%s:%d:CHECKPOINT\n",__FILE__,__LINE__); //XXX
//fprintf(stderr,"%s:%d:CHECKPOINT MARCO\n",__FILE__,__LINE__); //XXX
//fprintf(stderr,"%s:%d:CHECKPOINT POLO!\n",__FILE__,__LINE__); //XXX


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "Angel.h"

#define DEFAULT_W 640
#define DEFAULT_H 480
#define MAX_FPS 60
#define DIMENSIONS 3
#define WIREFRAME false

char title[128];
GLFWwindow * mainWindow = NULL;
int windowX = 0, windowY = 0, windowW = 0, windowH = 0;
double aspectRatio = DEFAULT_W/DEFAULT_H, FPS = 0;
double mouseX = 0, mouseY = 0;


////////////////////////////////////////////////////////////////////////////////


#include <math.h>
const unsigned int iterations = 3;
const unsigned int triangles = (DIMENSIONS == 2) ?
    pow(3,iterations):
    pow(4,iterations+1);
vec4 points[triangles*3], colors[triangles*3];
vec4 palette[32] = {
    // (8) basic
    vec4(0.000,0.000,0.000,1.000), // black
    vec4(0.000,0.000,1.000,1.000), // blue
    vec4(0.000,1.000,0.000,1.000), // green
    vec4(0.000,1.000,1.000,1.000), // cyan
    vec4(1.000,0.000,0.000,1.000), // red
    vec4(1.000,0.000,1.000,1.000), // magenta
    vec4(1.000,1.000,0.000,1.000), // yellow
    vec4(1.000,1.000,1.000,1.000), // white
    
    // (7) grayscale
    vec4(0.875,0.875,0.875,1.000), // lightest gray
    vec4(0.750,0.750,0.750,1.000), // lighter gray
    vec4(0.625,0.625,0.625,1.000), // light gray
    vec4(0.500,0.500,0.500,1.000), // gray
    vec4(0.375,0.375,0.375,1.000), // dark gray
    vec4(0.250,0.250,0.250,1.000), // darker gray
    vec4(0.125,0.125,0.125,1.000), // darkest gray
    
    // (17) CSS standard
    vec4(0.000,1.000,1.000,1.000), // aqua
    vec4(0.000,0.000,0.000,1.000), // black
    vec4(0.000,0.000,1.000,1.000), // blue
    vec4(1.000,0.000,1.000,1.000), // fuchsia
    vec4(0.502,0.502,0.502,1.000), // gray
    vec4(0.000,0.502,0.000,1.000), // green
    vec4(0.000,1.000,0.000,1.000), // lime
    vec4(0.502,0.000,0.000,1.000), // maroon
    vec4(0.000,0.000,0.502,1.000), // navy
    vec4(0.502,0.502,0.000,1.000), // olive
    vec4(1.000,0.647,0.000,1.000), // orange
    vec4(0.502,0.000,0.502,1.000), // purple
    vec4(1.000,0.000,0.000,1.000), // red
    vec4(0.753,0.753,0.753,1.000), // silver
    vec4(0.000,0.502,0.502,1.000), // teal
    vec4(1.000,1.000,1.000,1.000), // white
    vec4(1.000,1.000,0.000,1.000), // yellow
};
#define BASIC_BLACK   0
#define BASIC_BLUE    1
#define BASIC_GREEN   2
#define BASIC_CYAN    3
#define BASIC_RED     4
#define BASIC_MAGENTA 5
#define BASIC_YELLOW  6
#define BASIC_WHITE   7

#define GRAY_LIGHTEST 8
#define GRAY_LIGHTER  9
#define GRAY_LIGHT    10
#define GRAY          11
#define GRAY_DARK     12
#define GRAY_DARKER   13
#define GRAY_DARKEST  14

#define CSS_AQUA      15
#define CSS_BLACK     16
#define CSS_BLUE      17
#define CSS_FUCHSIA   18
#define CSS_GRAY      19
#define CSS_GREEN     20
#define CSS_LIME      21
#define CSS_MAROON    22
#define CSS_NAVY      23
#define CSS_OLIVE     24
#define CSS_ORANGE    25
#define CSS_PURPLE    26
#define CSS_RED       27
#define CSS_SILVER    28
#define CSS_TEAL      29
#define CSS_WHITE     30
#define CSS_YELLOW    31
unsigned int brushColor = BASIC_WHITE;


void initTriangle ( const vec4 a , const vec4 b , const vec4 c ) {
//    printf("bottom left   back   - A (%f,%f,%f)\n",a[0],a[1],a[2]);
//    printf("bottom right  back   - B (%f,%f,%f)\n",b[0],b[1],b[2]);
//    printf("top    middle back   - C (%f,%f,%f)\n",c[0],c[1],c[2]);
    static unsigned int i = 0;
    
    points[i] = a;
    colors[i] = palette[brushColor];
    ++i;
    
    points[i] = b;
    colors[i] = palette[brushColor];
    ++i;
    
    points[i] = c;
    colors[i] = palette[brushColor];
    ++i;
}


void init2DGasket ( const vec4 a , const vec4 b , const vec4 c , int count ) {
    if (count > 0) {
        vec4 m[3];
        m[0] = (a+b)/2.0;
        m[1] = (a+c)/2.0;
        m[2] = (b+c)/2.0;
        init2DGasket(a   ,m[0],m[1],count-1);
        init2DGasket(m[0],b   ,m[2],count-1);
        init2DGasket(m[1],m[2],c   ,count-1);
    }
    else initTriangle(a,b,c);
}


void initTetrahedron ( const vec4 a , const vec4 b , const vec4 c , const vec4 d ) {
//    printf("bottom left   back   - A (%f,%f,%f)\n",a[0],a[1],a[2]);
//    printf("bottom right  back   - B (%f,%f,%f)\n",b[0],b[1],b[2]);
//    printf("top    middle back   - C (%f,%f,%f)\n",c[0],c[1],c[2]);
//    printf("middle middle middle - D (%f,%f,%f)\n",d[0],d[1],d[2]);
    brushColor = BASIC_BLACK; //XXX
    initTriangle(a,b,c);
    brushColor = GRAY_DARKER; //XXX
//    brushColor = CSS_FUCHSIA; //XXX
    initTriangle(a,d,c);
    brushColor = GRAY_LIGHTER; //XXX
//    brushColor = CSS_OLIVE; //XXX
    initTriangle(a,b,d);
    brushColor = GRAY; //XXX
//    brushColor = CSS_PURPLE; //XXX
    initTriangle(b,c,d);
}


void init3DGasket ( const vec4 a , const vec4 b , const vec4 c , const vec4 d , int count ) {
    if (count > 0) {
        vec4 m[6];
        m[0] = (a+b)/2.0;
        m[1] = (a+c)/2.0;
        m[2] = (b+c)/2.0;
        m[3] = (a+d)/2.0;
        m[4] = (b+d)/2.0;
        m[5] = (c+d)/2.0;
        init3DGasket(a   ,m[0],m[1],m[3],count-1);
        init3DGasket(m[0],b   ,m[2],m[4],count-1);
        init3DGasket(m[1],m[2],c   ,m[5],count-1);
        init3DGasket(m[3],m[4],m[5],d   ,count-1);
    }
    else initTetrahedron(a,b,c,d);
}


void initFrame ( ) {
    const GLfloat r = 0.9;
    switch (DIMENSIONS) {
        case 2:
            init2DGasket(
                vec4(-r,-r,0.0,1.0),
                vec4( r,-r,0.0,1.0),
                vec4( 0, r,0.0,1.0),
                iterations
            );
            break;
        case 3:
            init3DGasket(
                vec4(-r,-r, r,1.0),
                vec4( r,-r, r,1.0),
                vec4( 0, r, r,1.0),
                vec4( 0, 0, 0,1.0),
                iterations
            );
            break;
        default:
            fprintf(stderr,"Unrenderable Dimension (%d)\n",DIMENSIONS);
            break;
    }
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
}


void drawFrame ( ) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
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
        case GLFW_PRESS:
        case GLFW_RELEASE: return;
    }
    switch (button) {
        // http://www.glfw.org/docs/latest/group__buttons.html
        // http://www.glfw.org/docs/latest/group__mods.html
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


void drawTitle ( ) {
    sprintf(
        &title[0],
        "%ld FPS | %dx%d (%d,%d)",
        (int) FPS,
        windowW,windowH,
        (int) mouseX, (int) mouseY
    );
    glfwSetWindowTitle(mainWindow,&title[0]);
}


void display ( ) {
    static unsigned long int frames = 0;
    FPS = frames/glfwGetTime();
    ++frames;

    glfwGetWindowPos (mainWindow,&windowX,&windowY);
    glfwGetWindowSize(mainWindow,&windowW,&windowH);
    
    drawTitle();
    drawFrame();
    glfwSwapBuffers(mainWindow);
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
        if (FPS < MAX_FPS) display();
        //glfwWaitEvents();
        glfwPollEvents();
    }
    cleanup();
    return EXIT_SUCCESS;
}
