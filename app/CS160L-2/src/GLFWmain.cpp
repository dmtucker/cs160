#define DEFAULT_W 640
#define DEFAULT_H 480
#define MAX_FPS   30

// David Tucker <dmtucker@ucsc.edu>

#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#define VOID(x) ((void *) x)
#define AXIS_X      vec4(1,0,0,0)
#define AXIS_Y      vec4(0,1,0,0)
#define AXIS_Z      vec4(0,0,1,0)
#define AXIS_ORIGIN vec4(0,0,0,1)

#include "colors.h"
#include "shapes.h"
#include "shader.hpp"
#include "extras.hpp"

#define FPS          (frames/glfwGetTime())
#define ASPECT_RATIO (windowW/(GLfloat) windowH)

#define ORTHOGRAPHIC  mat4(1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1) //XXX ortho(-10.0f,-10.0f,-10.0f,-10.0f,0.1f,10.0f)
#define PERSPECTIVE  perspective(lens,ASPECT_RATIO,0.1f,100.0f)

GLfloat lens = 60;
bool orthographic = false, stretch = false, culling = true, axes = true;

char title[128];
GLFWwindow * mainWindow = NULL;
int   windowX = 0, windowY = 0, windowW = DEFAULT_W, windowH = DEFAULT_H;
unsigned long int frames = 0, idle = 0;

GLuint program = 0, shaderPosition = 0, shaderColor = 0,
                    shaderModel    = 0, shaderView  = 0, shaderProjection = 0;
mat4 projection = (orthographic) ? ORTHOGRAPHIC : PERSPECTIVE;

GLuint axisVAO = 0, axisVBO = 0;

vec4 pEye = AXIS_ORIGIN, pFocus = vec4(0,0,-1,1), vUp = AXIS_Y;

vec3 pCursor = vec3(0,0,1), vCursor = vec3(0,0,1);

bool moveL = false, moveR = false, moveF = false, moveB = false;

////////////////////////////////////////////////////////////////////////////////


void printVec3 ( vec3 v ) { printf("(%6.3f,%6.3f,%6.3f)\n"      ,v.x,v.y,v.z); }
void printVec4 ( vec4 v ) { printf("(%6.3f,%6.3f,%6.3f,%6.3f)\n",v.x,v.y,v.z,v.w); }
void printMat4 ( mat4 v ) { printf("|%6.3f,%6.3f,%6.3f,%6.3f|\n",v[0].x,v[0].y,v[0].z,v[0].w);
                            printf("|%6.3f,%6.3f,%6.3f,%6.3f|\n",v[1].x,v[1].y,v[1].z,v[1].w);
                            printf("|%6.3f,%6.3f,%6.3f,%6.3f|\n",v[2].x,v[2].y,v[2].z,v[2].w);
                            printf("|%6.3f,%6.3f,%6.3f,%6.3f|\n",v[3].x,v[3].y,v[3].z,v[3].w); }


void Gram_Schmidt ( ) {
    vec4 vEye = normalize(pFocus-pEye); //XXX
    vUp = normalize(vUp-vEye*dot(vEye,vUp));
//    vRight = normalize(vRight-vEye*dot(vEye,vRight)-vUp*dot(vUp,vRight));
}


#include "snowman.cpp"
//#include "colorCube.cpp"
//#include "fractal.cpp"


void initFrame ( ) {
//    initColorCube();
//    initSierpinksi();
    initSnowman();
    
    pEye   = vec4(5,5,5,1);
    pFocus = AXIS_ORIGIN;
//    Gram_Schmidt();
}


void idleFrame ( ) {
    ++idle;
    
    static vec3 pPrevious = pCursor;
    vCursor   = pCursor-pPrevious;
    pPrevious = pCursor;
}


void drawFrame ( ) {
//    drawColorCube();
//    drawSierpinski();
    drawSnowman();
}


void cleanFrame ( ) {
    //XXX
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
        default: break;
        // http://www.glfw.org/docs/latest/group__keys.html
        // http://www.glfw.org/docs/latest/group__mods.html
        case GLFW_KEY_A: // move left / toggle axes
            if (mods == GLFW_MOD_CONTROL) {
                if (action == GLFW_PRESS) axes = not axes;
            }
            else {
                switch (action) {
                    case GLFW_PRESS:   moveL = true;  break;
                    case GLFW_RELEASE: moveL = false; break;
                    default: break;
                }
            }
            break;
        case GLFW_KEY_C: // toggle culling
            if (mods == GLFW_MOD_CONTROL) {
                if (action == GLFW_PRESS) {
                    culling = not culling;
                    if (culling) glEnable(GL_CULL_FACE);
                    else        glDisable(GL_CULL_FACE);
                    checkGL(__FILE__,__LINE__);
                }
            }
            break;
        case GLFW_KEY_D: // move right
            switch (action) {
                case GLFW_PRESS:   moveR = true;  break;
                case GLFW_RELEASE: moveR = false; break;
                default: break;
            }
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
        case GLFW_KEY_R: // toggle ratio mode
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
        case GLFW_KEY_S: // move back
            switch (action) {
                case GLFW_PRESS:   moveB = true;  break;
                case GLFW_RELEASE: moveB = false; break;
                default: break;
            }
            break;
        case GLFW_KEY_W: // move forward / toggle wireframe
            static bool wireframe = false;
            static bool framing   = false;
            if (mods == GLFW_MOD_CONTROL) {
                switch (action) {
                    case GLFW_PRESS:
                        framing = true;
                        wireframe = not wireframe;
                        if (wireframe) glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
                        else {
                            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
                        }
                        checkGL(__FILE__,__LINE__);
                        break;
                    case GLFW_RELEASE:
                        framing = false;
                    default: break;
                }
            }
            else {
                if (framing) {
                    if (action == GLFW_RELEASE) framing = false;
                }
                else switch (action) {
                    case GLFW_PRESS:   moveF = true;  break;
                    case GLFW_RELEASE: moveF = false; break;
                    default: break;
                }
            }
            break;
        
        //XXX temporary until FPS Camera
        case GLFW_KEY_UP:   // levitate up
            if (mods == GLFW_MOD_CONTROL) pFocus.y += (mods == GLFW_MOD_SHIFT) ? 0.5f : 0.1f;
            else                            pEye.y += (mods == GLFW_MOD_SHIFT) ? 0.5f : 0.1f;
            break;
        case GLFW_KEY_DOWN: // levitate down
            if (mods == GLFW_MOD_CONTROL) pFocus.y -= (mods == GLFW_MOD_SHIFT) ? 0.5f : 0.1f;
            else                            pEye.y -= (mods == GLFW_MOD_SHIFT) ? 0.5f : 0.1f;
            break;
        case GLFW_KEY_LEFT: // orbit left
            pEye = rotate(mat4(),(mods == GLFW_MOD_SHIFT) ? 2.5f : 0.5f,vec3(-AXIS_Y))*pEye;
            break;
        case GLFW_KEY_RIGHT: // orbit right
            pEye = rotate(mat4(),(mods == GLFW_MOD_SHIFT) ? 2.5f : 0.5f,vec3( AXIS_Y))*pEye;
            break;
        
        // snowman
        case GLFW_KEY_1: // wave right arm
            switch (action) {
                case GLFW_PRESS: wavingSnowman = not wavingSnowman;
                default: break;
            }
            break;
        case GLFW_KEY_2: //raise left arm
            switch (action) {
                case GLFW_PRESS:   raiseArm = true; break;
                case GLFW_RELEASE: raiseArm = false; break;
                default: break;
            }
            break;
        case GLFW_KEY_3: // lower left arm
            switch (action) {
                case GLFW_PRESS:   lowerArm = true; break;
                case GLFW_RELEASE: lowerArm = false; break;
                default: break;
            }
            break;
        case GLFW_KEY_4: //raise left elbow
            switch (action) {
                case GLFW_PRESS:   raiseElbow = true; break;
                case GLFW_RELEASE: raiseElbow = false; break;
                default: break;
            }
            break;
        case GLFW_KEY_5: // lower left elbow
            switch (action) {
                case GLFW_PRESS:   lowerElbow = true; break;
                case GLFW_RELEASE: lowerElbow = false; break;
                default: break;
            }
            break;
    }
}


static void click ( GLFWwindow * window , int button , int action , int mods ) {
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
        case GLFW_MOUSE_BUTTON_LEFT:
        case GLFW_MOUSE_BUTTON_MIDDLE:
        case GLFW_MOUSE_BUTTON_RIGHT:
        default: break;
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
    printf("OpenGL v%s\n",glGetString(GL_VERSION));
    
    (void) glGetError();
    
    glClearColor(0.2,0.2,0.2,1);
    
    program   = initShaders("src/vertex.glsl","src/fragment.glsl");
    glUseProgram(program);
    glShadeModel(GL_SMOOTH);
    checkGL(__FILE__,__LINE__);
    
    shaderPosition   = glGetAttribLocation( program,"vPosition");
    shaderColor      = glGetAttribLocation( program,"vColor");
    shaderModel      = glGetUniformLocation(program,"vModel");
    shaderView       = glGetUniformLocation(program,"vView");
    shaderProjection = glGetUniformLocation(program,"vProjection");
    glUniformMatrix4fv(shaderProjection,1,GL_FALSE,value_ptr(projection));
    checkGL(__FILE__,__LINE__);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    checkGL(__FILE__,__LINE__);
    
    if (culling) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        checkGL(__FILE__,__LINE__);
    }
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    checkGL(__FILE__,__LINE__);
}


void initAxes ( ) {
    GLfloat l = 10;
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
    glBufferData(GL_ARRAY_BUFFER,sizeof(axisXYZW)+sizeof(axisRGBA),NULL,GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER,0               ,sizeof(axisXYZW),axisXYZW);
    glBufferSubData(GL_ARRAY_BUFFER,sizeof(axisXYZW),sizeof(axisRGBA),axisRGBA);
    checkGL(__FILE__,__LINE__);
    
    glGenVertexArrays(1,&axisVAO);
    glBindVertexArray(axisVAO);
    checkGL(__FILE__,__LINE__);
    glEnableVertexAttribArray(shaderPosition);
    glVertexAttribPointer(shaderPosition,4,GL_FLOAT,GL_FALSE,0,VOID(0));
    checkGL(__FILE__,__LINE__);
    glEnableVertexAttribArray(shaderColor);
    glVertexAttribPointer(shaderColor,4,GL_FLOAT,GL_FALSE,0,VOID(sizeof(axisXYZW)));
    checkGL(__FILE__,__LINE__);
}


void initialize ( ) {
     initGLFW();
     initGLEW();
     initGL();
     initAxes();
     initFrame();
}


void meta ( ) {
    glfwGetWindowPos (mainWindow,&windowX,&windowY);
    glfwGetWindowSize(mainWindow,&windowW,&windowH);
    sprintf(
        &title[0],
        "%dx%d (%d,%d) | %ds @ %d FPS | %s %s%s",
        (int) windowW,   (int) windowH,
        (int) pCursor.x, (int) pCursor.y,
        (int) glfwGetTime(),
        (int) round(FPS),
        (orthographic) ? "orthographic" : "perspective",
        (stretch)      ? "stretch"      : "aspect",
        (culling)      ? ""             : " (unculled)"
    );
}


void drawAxes ( ) {
    glBindVertexArray(axisVAO);
    glUniformMatrix4fv(shaderModel,1,GL_FALSE,value_ptr(mat4()));
    glDrawArrays(GL_LINES,0,12);
}


void moveCamera ( ) {
    const GLfloat speed = 0.05;
    mat4 movement = mat4();
    if (moveL xor moveR) {
        movement = (moveL) ?
            translate(mat4(),speed*cross(vec3(vUp),vec3(normalize(pFocus-pEye)))):
            translate(mat4(),speed*cross(vec3(normalize(pFocus-pEye)),vec3(vUp)));
    }
    if (moveF xor moveB) {
        movement = (moveF) ?
            translate(mat4(),vec3(speed*normalize(pFocus-pEye))):
            translate(mat4(),vec3(speed*normalize(pEye-pFocus)));
    }
    pEye   = movement*pEye;
    pFocus = movement*pFocus;
    
/*/XXX FPS Camera
    static vec2 theta = vec2(0,0);       const double delta = 90.0;
//    theta.x += (GLfloat) linear_map(pCursor.y,0,windowH, delta,-delta);
//    theta.y += (GLfloat) linear_map(pCursor.x,0,windowW,-delta, delta);
    theta.x += (GLfloat) linear_map(proj(vCursor,vec3(AXIS_Y)).y,-windowH/2.0,windowH/2.0,-delta,delta);
    theta.y += (GLfloat) linear_map(proj(vCursor,vec3(AXIS_X)).x,-windowW/2.0,windowW/2.0,-delta,delta);
    if (theta.x <  -90) theta.x  = -90;
    if (theta.x >   90) theta.x  =  90;
    if (theta.y < -360) theta.y += 360;
    if (theta.y >  360) theta.y -= 360;
    GLfloat inclination = (theta.x+180)*DEG2RAD;
    GLfloat azimuth     = (theta.y+ 90)*DEG2RAD;
    vec3 vLook = vec3(
          sin(azimuth)*cos(inclination),
        -(sin(azimuth)*sin(inclination)),
          cos(azimuth)
    );
//    pFocus = translate(mat4(),pEye)*vec4(normalize(pEye+pLook),0));
    pFocus = translate(mat4(),vec3(pEye))*vec4(vLook,0);
/**/
    
//    Gram_Schmidt();
    glUniformMatrix4fv(shaderView,1,GL_FALSE,value_ptr(lookAt(vec3(pEye),vec3(pFocus),vec3(vUp))));
}


void cleanAxes ( ) {
    //XXX
}


void cleanGL ( ) {
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}


void cleanGLFW ( ) {
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}


void cleanup ( ) {
    cleanFrame();
    cleanAxes();
    cleanGL();
    cleanGLFW();
}


int main ( int argc , char * argv[] ) {
    initialize();
    
    puts("Controls\n");
    puts("a       move left");
    puts("d       move right");
    puts("s       move forward");
    puts("w       move backward");
    putchar('\n');
    puts("ctrl-a  toggle axes");
    puts("ctrl-c  toggle culling");
    puts("ctrl-p  toggle projection mode (orthographic/perspective)");
    puts("ctrl-q  quit");
    puts("ctrl-r  toggle ratio mode (stretch/aspect)");
    puts("ctrl-w  toggle wireframing");
    putchar('\n');
    puts("up      levitate up the y-axis");
    puts("down    levitate down the y-axis");
    puts("left    orbit  CW around the y-axis");
    puts("right   orbit CCW around the y-axis");
    putchar('\n');
    puts("1       make the snowman wave");
    puts("2       raise the snowman's left arm");
    puts("3       lower the snowman's left arm");
    puts("4       raise the snowman's left forearm");
    puts("5       lower the snowman's left forearm");
    
    while (not glfwWindowShouldClose(mainWindow)) {
//        if (glfwGetTime() > 600) glfwSetWindowShouldClose(mainWindow,GL_TRUE); //XXX
        meta();
        if (FPS > MAX_FPS) idleFrame();
        else {
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
            if (axes) drawAxes();
            moveCamera();
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


//fprintf(stderr,"%s:%d:CHECKPOINT\n",__FILE__,__LINE__); //XXX
//fprintf(stderr,"%s:%d:MARCO\n",__FILE__,__LINE__); //XXX
//fprintf(stderr,"%s:%d:POLO!\n",__FILE__,__LINE__); //XXX
