char title[128];

#define DEFAULT_W 640
#define DEFAULT_H 480

GLFWwindow * defaultWindow  = NULL;
int
    windowX = 0, windowW = DEFAULT_W,
    windowY = 0, windowH = DEFAULT_H;

#define ASPECT_RATIO (windowW/(GLfloat) windowH)

unsigned long int frames = 0;

#define     FPS (frames/glfwGetTime())
#define MAX_FPS 30

bool
    cull = true,
    wire = false;

glm::vec3
    pCursor = glm::vec3(0,0,1),
    vCursor = glm::vec3(0,0,1);


void initGLFW ( ) {
    static bool uninitialized = true;
    if (uninitialized) {

        (void) glfwSetErrorCallback(error);

        if (not glfwInit()) {
            fputs("GLFW Initialization Failure",stderr);
            exit(EXIT_FAILURE);
        }

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,GL_TRUE); //XXX
        glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
        defaultWindow = glfwCreateWindow(DEFAULT_W,DEFAULT_H,&title[0],NULL,NULL);
        if (defaultWindow == NULL) {
            glfwTerminate();
            fputs("Window Creation Failure",stderr);
            exit(EXIT_FAILURE);
        }
        glfwMakeContextCurrent(defaultWindow);

        (void)  glfwSetWindowSizeCallback(defaultWindow,resize);
        (void)         glfwSetKeyCallback(defaultWindow,keyboard);
        (void) glfwSetMouseButtonCallback(defaultWindow,click);
        (void)   glfwSetCursorPosCallback(defaultWindow,hover);
        (void)      glfwSetScrollCallback(defaultWindow,scroll);

        printf("GLFW v%s\n",glfwGetVersionString());
    }
    else {
        glfwDestroyWindow(defaultWindow);
        glfwTerminate();
    }
    uninitialized = not uninitialized;
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
        else (void) glGetError();

        printf("GLEW v%s\n",glewGetString(GLEW_VERSION));
    }
    uninitialized = not uninitialized;
}


bool checkGL ( const char * file , GLuint line , bool required = true ) {
    const GLenum status = glGetError();
    if (status != GL_NO_ERROR) {
        fprintf(stderr,"%s:%d %s\n",file,line,gluErrorString(status));
        if (required) exit(EXIT_FAILURE);
        return false;
    }
    return true;
}


void initGL ( ) {
    static bool uninitialized = true;
    if (uninitialized) {

        glClearColor(0.1,0.1,0.1,1.0);
        glClearDepth(1);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

        glDisable(GL_DITHER);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        checkGL(__FILE__,__LINE__);
        printf("OpenGL v%s\n",glGetString(GL_VERSION));
    }
    else {
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }
    uninitialized = not uninitialized;
}


void initialize ( ) {
    static bool uninitialized = true;
    if (uninitialized) {
        initGLFW();
        initGLEW();
        initGL();
        initShaders();
        initOrientation();
        initFrame();
    }
    else {
        initFrame();
        initOrientation();
        initShaders();
        initGL();
        initGLEW();
        initGLFW();
    }
    uninitialized = not uninitialized;
}
