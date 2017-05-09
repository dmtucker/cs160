#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ppm_canvas.h" //XXX
#include "stb_image.c"

// David Tucker <dmtucker@ucsc.edu> ////////////////////////////////////////////

#define CHECKPOINT fprintf(stderr,"%s:%d CHECKPOINT\n",__FILE__,__LINE__);\
                   checkGL(__FILE__,__LINE__);

#define VOID(x) ((void *) (x))

#include "extras.hpp"

 /* XXX relocate? */
canvas_t terrain, texture;
//XXX glm::rotate(glm::mat4(),(GLfloat) (((GLint) glfwGetTime())%360),glm::vec3(AXIS_Z));
/**/

#include     "globals.h"
#include        "init.cpp"
#include     "shaders.cpp"
#include "orientation.cpp"
#include       "frame.cpp"
#include          "IO.cpp"


int main ( int argc , char * argv[] ) {

    if (argc < 3) {
        fprintf(stderr,"usage: %s [terrain] [texture]\n",argv[0]);
        return EXIT_FAILURE;
    }
    if (ppmLoadCanvas(argv[1],&terrain) != 0) puts("Terrain Load Failure"); //XXX
    if (ppmLoadCanvas(argv[2],&texture) != 0) puts("Texture Load Failure"); //XXX

    initialize();
    while (not glfwWindowShouldClose(defaultWindow)) {

        glfwGetWindowPos (defaultWindow,&windowX,&windowY);
        glfwGetWindowSize(defaultWindow,&windowW,&windowH);
        checkGL(__FILE__,__LINE__);

        if (FPS <= MAX_FPS) {

            static glm::vec3
                pPrevious = pCursor;
                vCursor = pCursor-pPrevious;
                pPrevious = pCursor;

            reorient();
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
            drawTitle();
            drawFrame();
            glfwSwapBuffers(defaultWindow);
            ++frames;
        }

        glfwPollEvents();
    }
    initialize();
    return EXIT_SUCCESS;
}
