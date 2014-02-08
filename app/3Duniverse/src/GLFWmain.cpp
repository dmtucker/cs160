#define DEFAULT_W 640
#define DEFAULT_H 480
#define MAX_FPS    30

// David Tucker <dmtucker@ucsc.edu>

#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/orthonormalize.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include "colors.h"
#include "shapes.h"
#include "shader.hpp"
#include "extras.hpp"

#define ASPECT_RATIO (windowW/(GLfloat) windowH)
#define FPS          (frames/glfwGetTime())
#define VOID(x)      ((void *) x)

char title[128];
GLFWwindow * mainWindow = NULL;
int   windowX = 0, windowY = 0, windowW = DEFAULT_W, windowH = DEFAULT_H;
unsigned long int frames = 0;

vec3 pCursor = vec3(0,0,1), vCursor = vec3(0,0,1);

#define FAR 512.0f
#define GRID_DENSITY 32 // lines per signed axis
#define ORTHOGRAPHIC ortho(-FAR/10,FAR/10,-FAR/10,FAR/10,0.1f,FAR)
#define PERSPECTIVE  perspective(lens,ASPECT_RATIO,0.1f,FAR)

GLfloat lens = 60; // degrees
bool axis         = false,
     grid         = true,
     culling      = true,
     orthographic = false,
     stretch      = false,
     wireframe    = false;

GLuint shaderPosition = 0, shaderColor = 0,
       shaderModel    = 0, shaderView  = 0, shaderProjection = 0;
mat4 projection = (orthographic) ? ORTHOGRAPHIC : PERSPECTIVE;

GLuint axisVAO = 0, axisVBO = 0, gridVAO = 0, gridVBO = 0;

vec4 pEye = AXIS_ORIGIN, pFocus = vec4(0,0,-1,1), vUp = AXIS_Y;
bool moveL = false, moveR = false,
     moveB = false, moveF = false,
     moveD = false, moveU = false,
     lookD = false, lookU = false,
     lookL = false, lookR = false;

////////////////////////////////////////////////////////////////////////////////


#include "colorCube.cpp"
#include "fractal.cpp"


void initFrame ( ) {
    static bool uninitialized = true;
    if (uninitialized) {
        
        initColorCube();
        initSierpinski();
        
        int s = 10;
        colorCubeModel = scale(colorCubeModel,vec3(s,s,s));
        gasketModel    = scale(gasketModel   ,vec3(s,s,s));
        
        pEye   = vec4(s,2*s,s,1);
        pFocus = AXIS_ORIGIN;
        
        uninitialized = false;
    }
    else {
        
        initSierpinski();
        initColorCube();
        
        uninitialized = true;
    }
}


void drawFrame ( ) {
//    drawColorCube();
    drawSierpinski();
}


////////////////////////////////////////////////////////////////////////////////


static void error ( int error , const char * description ) { fputs(description,stderr); }


static void resize ( GLFWwindow * window , int width , int height ) {
    glViewport(0,0,width,height);
    checkGL(__FILE__,__LINE__);
    if (not stretch) {
        projection = (orthographic) ? ORTHOGRAPHIC : PERSPECTIVE;
        glUniformMatrix4fv(shaderProjection,1,GL_FALSE,value_ptr(projection));
    }
}


static void keyboard ( GLFWwindow * window , int key , int scancode , int action , int mods ) {
    switch (key) {
        default: return;
        // http://www.glfw.org/docs/latest/group__keys.html
        // http://www.glfw.org/docs/latest/group__mods.html
        
        case GLFW_KEY_A: // toggle axis
            if (mods == GLFW_MOD_CONTROL and action == GLFW_PRESS) axis = not axis;
            break;
        
        case GLFW_KEY_C: // toggle culling
            if (mods == GLFW_MOD_CONTROL and action == GLFW_PRESS) {
                culling = not culling;
                if (culling) glEnable(GL_CULL_FACE);
                else        glDisable(GL_CULL_FACE);
                checkGL(__FILE__,__LINE__);
            }
            break;
        
        case GLFW_KEY_G: // toggle grid
            if (mods == GLFW_MOD_CONTROL and action == GLFW_PRESS) grid = not grid;
            break;
        
        case GLFW_KEY_SLASH: if (mods != GLFW_MOD_SHIFT) break;
        case GLFW_KEY_H: // help
            puts("Basic Maneuvers");
            puts("arrows    look around");
            putchar('\n');
            puts("key-2     move backward (retreat)");
            puts("key-4     move left      (strafe)");
            puts("key-6     move right     (strafe)");
            puts("key-8     move forward  (proceed)");
            puts("key-minus move down     (descend)");
            puts("key-plus  move up        (ascend)");
            putchar('\n');
            puts("Controls");
            puts("ctrl-a    toggle axis");
            puts("ctrl-c    toggle culling");
            puts("ctrl-g    toggle grid");
            puts("ctrl-h    help");
            puts("ctrl-p    toggle projection mode (orthographic/perspective)");
            puts("ctrl-q    quit");
            puts("ctrl-s    toggle scaling mode (stretch/aspect)");
            puts("ctrl-w    toggle wireframing");
            puts("ctrl-?    show usage/options");
            break;
        
        case GLFW_KEY_P: // toggle projection mode
            if (mods == GLFW_MOD_CONTROL) {
                if (action == GLFW_PRESS) {
                    orthographic = not orthographic;
                    projection = (orthographic) ? ORTHOGRAPHIC : PERSPECTIVE;
                    glUniformMatrix4fv(shaderProjection,1,GL_FALSE,value_ptr(projection));
                }
            }
            break;
        
        case GLFW_KEY_Q: // quit
            if (mods == GLFW_MOD_CONTROL) {
                if (action == GLFW_PRESS) glfwSetWindowShouldClose(window,GL_TRUE);
            }
            break;
        
        case GLFW_KEY_S: // toggle scaling mode
            if (mods == GLFW_MOD_CONTROL) {
                if (action == GLFW_PRESS) {
                    stretch = not stretch;
                    if (not stretch) {
                        projection = (orthographic) ? ORTHOGRAPHIC : PERSPECTIVE;
                        glUniformMatrix4fv(shaderProjection,1,GL_FALSE,value_ptr(projection));
                    }
                }
            }
            break;
        
        case GLFW_KEY_W: // toggle wireframe
            if (mods == GLFW_MOD_CONTROL and action == GLFW_PRESS) {
                wireframe = not wireframe;
                if (wireframe) glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
                else           glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
                checkGL(__FILE__,__LINE__);
            }
            break;
        
        case GLFW_KEY_KP_2: // move backward
            switch (action) {
                case GLFW_PRESS:   moveB = true;  break;
                case GLFW_RELEASE: moveB = false; break;
                default: break;
            }
            break;
        
        case GLFW_KEY_KP_4: // move left
            switch (action) {
                case GLFW_PRESS:   moveL = true;  break;
                case GLFW_RELEASE: moveL = false; break;
                default: break;
            }
            break;
        
        case GLFW_KEY_KP_6: // move right
            switch (action) {
                case GLFW_PRESS:   moveR = true;  break;
                case GLFW_RELEASE: moveR = false; break;
                default: break;
            }
            break;
        
        case GLFW_KEY_KP_8: // move forward
            switch (action) {
                case GLFW_PRESS:   moveF = true;  break;
                case GLFW_RELEASE: moveF = false; break;
                default: break;
            }
            break;
        
        case GLFW_KEY_KP_ADD: // move up
            switch (action) {
                case GLFW_PRESS:   moveU = true;  break;
                case GLFW_RELEASE: moveU = false; break;
                default: break;
            }
            break;
        
        case GLFW_KEY_KP_SUBTRACT: // move down
            switch (action) {
                case GLFW_PRESS:   moveD = true;  break;
                case GLFW_RELEASE: moveD = false; break;
                default: break;
            }
            break;
        
        case GLFW_KEY_LEFT: // look left
            switch (action) {
                case GLFW_PRESS:   lookL = true;  break;
                case GLFW_RELEASE: lookL = false; break;
                default: break;
            }
            break;
        
        case GLFW_KEY_RIGHT: // look right
            switch (action) {
                case GLFW_PRESS:   lookR = true;  break;
                case GLFW_RELEASE: lookR = false; break;
                default: break;
            }
            break;
        
        case GLFW_KEY_DOWN: // look down
            switch (action) {
                case GLFW_PRESS:   lookD = true;  break;
                case GLFW_RELEASE: lookD = false; break;
                default: break;
            }
            break;
        
        case GLFW_KEY_UP: // look up
            switch (action) {
                case GLFW_PRESS:   lookU = true;  break;
                case GLFW_RELEASE: lookU = false; break;
                default: break;
            }
            break;
    }
}


static void click ( GLFWwindow * window , int button , int action , int mods ) {
    switch (button) {
        default: return;
        // http://www.glfw.org/docs/latest/group__buttons.html
        // http://www.glfw.org/docs/latest/group__mods.html
        case GLFW_MOUSE_BUTTON_LEFT:
        case GLFW_MOUSE_BUTTON_MIDDLE:
        case GLFW_MOUSE_BUTTON_RIGHT:  break;
    }
}


static void updateCursor ( GLFWwindow * window , double xpos , double ypos ) {
    vCursor = vec3(xpos,ypos,1)-pCursor;
    pCursor = vec3(xpos,ypos,1);
}


static void scroll ( GLFWwindow * window , double xoffset , double yoffset ) {
    //XXX Scale the view!
}


////////////////////////////////////////////////////////////////////////////////


void initGLFW ( ) {
    static bool uninitialized = true;
    if (uninitialized) {
    
        glfwSetErrorCallback(error);
        if (not glfwInit()) {
            fputs("GLFW Initialization Error",stderr);
            exit(EXIT_FAILURE);
        }
        
        glfwDefaultWindowHints();
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
        glfwSetMouseButtonCallback(mainWindow,click);
        glfwSetCursorPosCallback(mainWindow,updateCursor);
        glfwSetScrollCallback(mainWindow,scroll);
        
        printf("GLFW v%s\n",glfwGetVersionString());
        
        uninitialized = false;
    }
    else {
    
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        
        uninitialized = true;
    }
}


void initGLEW ( ) {
    static bool uninitialized = true;
    if (uninitialized) {
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
        uninitialized = false;
    }
    else uninitialized = true;
}


void initGL ( ) {
    
    static GLuint defaultProgram;
    
    static bool uninitialized = true;
    if (uninitialized) {
        
        (void) glGetError();
        printf("OpenGL v%s\n",glGetString(GL_VERSION));
        
        glClearColor(0.2,0.2,0.2,1);
        defaultProgram = shaderProgram("src/vertex.glsl","src/fragment.glsl");
        glUseProgram(defaultProgram);
        checkGL(__FILE__,__LINE__);
        
        glShadeModel(GL_SMOOTH);
        shaderPosition   = glGetAttribLocation( defaultProgram,"vPosition");
        shaderColor      = glGetAttribLocation( defaultProgram,"vColor");
        shaderModel      = glGetUniformLocation(defaultProgram,"vModel");
        shaderView       = glGetUniformLocation(defaultProgram,"vView");
        shaderProjection = glGetUniformLocation(defaultProgram,"vProjection");
        glUniformMatrix4fv(shaderProjection,1,GL_FALSE,value_ptr(projection));
        checkGL(__FILE__,__LINE__);
        
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
        
        uninitialized = false;
    }
    else {
        
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        
        glUseProgram(0);
        glDeleteProgram(defaultProgram);
        checkGL(__FILE__,__LINE__);
        
        uninitialized = true;
    }
}


void initAxis ( ) {
    static bool uninitialized = true;
    if (uninitialized) {
    
        GLfloat l = FAR;
        vec4 axisXYZW[12] = {
            AXIS_ORIGIN,vec4( l, 0, 0, 1), // +x
            AXIS_ORIGIN,vec4(-l, 0, 0, 1), // -x
            AXIS_ORIGIN,vec4( 0, l, 0, 1), // +y
            AXIS_ORIGIN,vec4( 0,-l, 0, 1), // -y
            AXIS_ORIGIN,vec4( 0, 0, l, 1), // +z
            AXIS_ORIGIN,vec4( 0, 0,-l, 1)  // -z
        };
        vec4 axisRGBA[12] = {
            palette[BASIC_RED]  ,palette[BASIC_WHITE], // +x
            palette[BASIC_RED]  ,palette[BASIC_BLACK], // -x
            palette[BASIC_GREEN],palette[BASIC_WHITE], // +y
            palette[BASIC_GREEN],palette[BASIC_BLACK], // -y
            palette[BASIC_BLUE] ,palette[BASIC_WHITE], // +z
            palette[BASIC_BLUE] ,palette[BASIC_BLACK]  // -z
        };
        
        glGenBuffers(1,&axisVBO);
        glBindBuffer(GL_ARRAY_BUFFER,axisVBO);
        glBufferData(   GL_ARRAY_BUFFER,sizeof(axisXYZW)+sizeof(axisRGBA),NULL,GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER,0               ,sizeof(axisXYZW),axisXYZW);
        glBufferSubData(GL_ARRAY_BUFFER,sizeof(axisXYZW),sizeof(axisRGBA),axisRGBA);
        checkGL(__FILE__,__LINE__);
        
        glGenVertexArrays(1,&axisVAO);
        glBindVertexArray(axisVAO);
        glEnableVertexAttribArray(shaderPosition);
        glEnableVertexAttribArray(shaderColor);
        glVertexAttribPointer(shaderPosition,4,GL_FLOAT,GL_FALSE,0,VOID(0));
        glVertexAttribPointer(shaderColor   ,4,GL_FLOAT,GL_FALSE,0,VOID(sizeof(axisXYZW)));
        checkGL(__FILE__,__LINE__);
        
        uninitialized = false;
    }
    else {
        
        glBindVertexArray(0);
        glDeleteVertexArrays(1,&axisVAO);
        glDeleteBuffers(1,&axisVBO);
        checkGL(__FILE__,__LINE__);
        
        uninitialized = true;
    }
}


void initGrid ( ) {
    static bool uninitialized = true;
    if (uninitialized) {
        
        GLfloat l = FAR;
        vec4 gridXYZW[16*(int) GRID_DENSITY], gridRGBA[16*(int) GRID_DENSITY];
        GLfloat delta = l/GRID_DENSITY;
        int gridI = 0;
        for (int i=0; i < GRID_DENSITY ;++i) {
            
            // -X
            gridXYZW[gridI] = vec4(-i*delta,0,0,1);
            gridRGBA[gridI] = palette[BASIC_WHITE];
                   ++gridI;
            gridXYZW[gridI] = vec4(-i*delta,0,-l,1);
            gridRGBA[gridI] = palette[BASIC_WHITE];
                   ++gridI;
            gridXYZW[gridI] = vec4(-i*delta,0,0,1);
            gridRGBA[gridI] = palette[BASIC_WHITE];
                   ++gridI;
            gridXYZW[gridI] = vec4(-i*delta,0,l,1);
            gridRGBA[gridI] = palette[BASIC_WHITE];
                   ++gridI;
            
            // +X
            gridXYZW[gridI] = vec4(i*delta,0,0,1);
            gridRGBA[gridI] = palette[BASIC_WHITE];
                   ++gridI;
            gridXYZW[gridI] = vec4(i*delta,0,-l,1);
            gridRGBA[gridI] = palette[BASIC_WHITE];
                   ++gridI;
            gridXYZW[gridI] = vec4(i*delta,0,0,1);
            gridRGBA[gridI] = palette[BASIC_WHITE];
                   ++gridI;
            gridXYZW[gridI] = vec4(i*delta,0,l,1);
            gridRGBA[gridI] = palette[BASIC_WHITE];
                   ++gridI;
            
            // -Z
            gridXYZW[gridI] = vec4(0,0,-i*delta,1);
            gridRGBA[gridI] = palette[BASIC_WHITE];
                   ++gridI;
            gridXYZW[gridI] = vec4(-l,0,-i*delta,1);
            gridRGBA[gridI] = palette[BASIC_WHITE];
                   ++gridI;
            gridXYZW[gridI] = vec4(0,0,-i*delta,1);
            gridRGBA[gridI] = palette[BASIC_WHITE];
                   ++gridI;
            gridXYZW[gridI] = vec4(l,0,-i*delta,1);
            gridRGBA[gridI] = palette[BASIC_WHITE];
                   ++gridI;
            
            // +Z
            gridXYZW[gridI] = vec4(0,0,i*delta,1);
            gridRGBA[gridI] = palette[BASIC_WHITE];
                   ++gridI;
            gridXYZW[gridI] = vec4(-l,0,i*delta,1);
            gridRGBA[gridI] = palette[BASIC_WHITE];
                   ++gridI;
            gridXYZW[gridI] = vec4(0,0,i*delta,1);
            gridRGBA[gridI] = palette[BASIC_WHITE];
                   ++gridI;
            gridXYZW[gridI] = vec4(l,0,i*delta,1);
            gridRGBA[gridI] = palette[BASIC_WHITE];
                   ++gridI;
        }
        
        glGenBuffers(1,&gridVBO);
        glBindBuffer(GL_ARRAY_BUFFER,gridVBO);
        glBufferData(   GL_ARRAY_BUFFER,sizeof(gridXYZW)+sizeof(gridRGBA),NULL,GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER,0               ,sizeof(gridXYZW),gridXYZW);
        glBufferSubData(GL_ARRAY_BUFFER,sizeof(gridXYZW),sizeof(gridRGBA),gridRGBA);
        checkGL(__FILE__,__LINE__);
        
        glGenVertexArrays(1,&gridVAO);
        glBindVertexArray(gridVAO);
        glEnableVertexAttribArray(shaderPosition);
        glEnableVertexAttribArray(shaderColor);
        glVertexAttribPointer(shaderPosition,4,GL_FLOAT,GL_FALSE,0,VOID(0));
        glVertexAttribPointer(shaderColor   ,4,GL_FLOAT,GL_FALSE,0,VOID(sizeof(gridXYZW)));
        checkGL(__FILE__,__LINE__);
        
        uninitialized = false;
    }
    else {
        
        glBindVertexArray(0);
        glDeleteVertexArrays(1,&gridVAO);
        glDeleteBuffers(1,&gridVBO);
        checkGL(__FILE__,__LINE__);
        
        uninitialized = true;
    }
}


void initialize ( ) {
    static bool uninitialized = true;
    if (uninitialized) {
        
        initGLFW();
        initGLEW();
        initGL();
        initAxis();
        initGrid();
        initFrame();
        
        uninitialized = false;
    }
    else {
        
        initFrame();
        initGrid();
        initAxis();
        initGL();
        initGLEW();
        initGLFW();
        
        uninitialized = true;
    }
}


void meta ( ) {
    
    glfwGetWindowPos (mainWindow,&windowX,&windowY);
    glfwGetWindowSize(mainWindow,&windowW,&windowH);
    
    static vec3 pPrevious = pCursor;
    vCursor   = pCursor-pPrevious;
    pPrevious = pCursor;
}


void moveCamera ( ) {
    
    vec4 vForward = normalize(pFocus-pEye);
    vec4 vRight   = vec4(cross(vec3(vForward),vec3(vUp)),0);
    pFocus = vForward+pEye;
    
    const GLfloat agility = 1.0;
    mat4 movement = mat4();
    if (moveR xor moveL) {
        movement = (moveR) ?
            translate(movement,vec3( vRight*agility)):
            translate(movement,vec3(-vRight*agility));
    }
    if (moveF xor moveB) {
        movement = (moveF) ?
            translate(movement,vec3( vForward*agility)):
            translate(movement,vec3(-vForward*agility));
    }
    if (moveU xor moveD) {
        movement = (moveU) ?
            translate(movement,vec3( vUp*agility)):
            translate(movement,vec3(-vUp*agility));
    }
    pEye   = movement*pEye;
    pFocus = movement*pFocus;
    
//XXX This should use the magnitude of vCursor.
//    bool flagDU = false, flagLR = false;;
//         if (vCursor.y > 0 and not lookD) { flagDU = lookD = true; }
//    else if (vCursor.y < 0 and not lookU) { flagDU = lookU = true; }
//         if (vCursor.x > 0 and not lookR) { flagLR = lookR = true; }
//    else if (vCursor.x < 0 and not lookL) { flagLR = lookL = true; }
    
    const GLfloat sensitivity = 1.5;
    if (lookU xor lookD) {
        vForward = normalize(rotate(mat4(),(lookU) ? sensitivity : -sensitivity,vec3(vRight))*vForward);
//        if (flagDU) flagDU = lookD = lookU = false;
    }
    if (lookL xor lookR) {
        vForward = rotate(mat4(),(lookL) ? sensitivity : -sensitivity,vec3(vUp))*vForward;
//        if (flagLR) flagLR = lookL = lookR = false;
    }
    pFocus = pEye+vForward;
    
    glUniformMatrix4fv(shaderView,1,GL_FALSE,value_ptr(lookAt(vec3(pEye),vec3(pFocus),vec3(vUp))));
}


void drawTitle ( ) {
    sprintf(
        &title[0],
        "%dx%d (%d,%d) | %ds @ %d FPS | %s%s %s%s",
        (int) windowW,   (int) windowH,
        (int) pCursor.x, (int) pCursor.y,
        (int) glfwGetTime(),
        (int) round(FPS),
        (wireframe)    ? "wireframe "   : "",
        (orthographic) ? "orthographic" : "perspective",
        (stretch)      ? "stretch"      : "aspect",
        (culling)      ? ""             : " (unculled)"
    );
    glfwSetWindowTitle(mainWindow,&title[0]);
}


void drawGrid ( ) {
    if (axis) {
        glBindVertexArray(axisVAO);
        glUniformMatrix4fv(shaderModel,1,GL_FALSE,value_ptr(mat4()));
        glDrawArrays(GL_LINES,0,12);
    }
    if (grid) {
        glBindVertexArray(gridVAO);
        glUniformMatrix4fv(shaderModel,1,GL_FALSE,value_ptr(mat4()));
        glDrawArrays(GL_LINES,0,16*GRID_DENSITY);
    }
}


int main ( int argc , char * argv[] ) {
    initialize();
    while (not glfwWindowShouldClose(mainWindow)) {
        if (FPS <= MAX_FPS) {
            meta();
            moveCamera();
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
            drawTitle();
            drawGrid();
            drawFrame();
            glfwSwapBuffers(mainWindow);
            ++frames;
        }
        glfwPollEvents();
    }
    initialize();
    return EXIT_SUCCESS;
}


//fprintf(stderr,"%s:%d:CHECKPOINT\n",__FILE__,__LINE__); //XXX
//fprintf(stderr,"%s:%d:MARCO\n",__FILE__,__LINE__); //XXX
//fprintf(stderr,"%s:%d:POLO!\n",__FILE__,__LINE__); //XXX
