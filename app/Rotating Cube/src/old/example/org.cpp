#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_W 640
#define DEFAULT_H 480
#define REACTIVE false // wait (true) or poll (false)
#define MAX_FPS 60 // Hz

char title[128];
GLFWwindow * mainWindow = NULL;
int windowX = 0, windowY = 0, windowW = 0, windowH = 0;
double aspectRatio = DEFAULT_W/DEFAULT_H, FPS = MAX_FPS;
double mouseX = 0, mouseY = 0;


////////////////////////////////////////////////////////////////////////////////


void initDisplay ( ) {
    //XXX
}


void drawFrame ( ) {
    float ratio;
    int width, height;
    glfwGetFramebufferSize(mainWindow, &width, &height);
    ratio = width / (float) height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
    glBegin(GL_TRIANGLES);
    glColor3f(1.f, 0.f, 0.f);
    glVertex3f(-0.6f, -0.4f, 0.f);
    glColor3f(0.f, 1.f, 0.f);
    glVertex3f(0.6f, -0.4f, 0.f);
    glColor3f(0.f, 0.f, 1.f);
    glVertex3f(0.f, 0.6f, 0.f);
    glEnd();
}


void cleanDisplay ( ) {
    //XXX
}


////////////////////////////////////////////////////////////////////////////////


static void error ( int error , const char * description ) {
    fputs(description,stderr);
}


void resize ( GLFWwindow * window , int width , int height ) {
    glViewport(0,0,width,height);
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


void mouse ( GLFWwindow * window , int button , int action , int mods ) {
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


void cursor ( GLFWwindow * window , double xpos , double ypos ) {
    mouseX = xpos;
    mouseY = ypos;
}


void scroll ( GLFWwindow * window , double xoffset , double yoffset ) {
    //XXX
}


void initGLFW ( ) {
    glfwSetErrorCallback(error);
    if (not glfwInit()) {
        fputs("GLFW Initialization Error",stderr);
        exit(EXIT_FAILURE);
    }
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,GL_TRUE); //XXX
    //glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    mainWindow = glfwCreateWindow(DEFAULT_W,DEFAULT_H,&title[0],NULL,NULL);
    if (mainWindow == NULL) {
        glfwTerminate();
        fputs("Window Creation Error",stderr);
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(mainWindow);

    //XXX glfwSetWindowSizeCallback(mainWindow,resize); // screen coordinates
    glfwSetFramebufferSizeCallback(mainWindow,resize); // pixels
    glfwSetKeyCallback(mainWindow,keyboard);
    glfwSetMouseButtonCallback(mainWindow,mouse);
    glfwSetCursorPosCallback(mainWindow,cursor);
    glfwSetScrollCallback(mainWindow,scroll);
    
    printf("GLFW v%s\n",glfwGetVersionString());
}


void initGL ( ) {
    (void) glGetError();
    
    printf("OpenGL v%s\n",glGetString(GL_VERSION));
    
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


void initialize () {
    initGLFW();
    //initGLEW();
    initGL();
    initDisplay();
}


bool limitFPS ( ) {
    static double last = 0;
    clock_t now = glfwGetTime();
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
    glfwSetWindowTitle(mainWindow,&title[0]);
}


void display ( ) {
//    if (limitFPS()) return;
//    else {
        static unsigned long int frames = 0;
        ++frames;
        FPS = frames/glfwGetTime();
//    }

    glfwGetWindowPos (mainWindow,&windowX,&windowY);
    glfwGetWindowSize(mainWindow,&windowW,&windowH);
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    drawTitle();
    drawFrame();
    
    glfwSwapBuffers(mainWindow);
}


void cleanGL ( ) {
    //XXX
//    glDisable(GL_CULL_FACE);
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
        display();
        if (REACTIVE) glfwPollEvents();
        else          glfwWaitEvents();
    }
    cleanup();
    return EXIT_SUCCESS;
}

//fprintf(stderr,"%s:%d:CHECKPOINT\n",__FILE__,__LINE__); //XXX
