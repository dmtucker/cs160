#ifndef GLOBALS_H
#define GLOBALS_H

#include <glm/glm.hpp>

#include    "colors.h"
#include "materials.h"


// init.cpp
void initGLFW   ( );
void initGLEW   ( );
void initGL     ( );
void initialize ( );

bool checkGL ( const char * file , GLuint line , bool required );


// shaders.cpp
void initShaders ( );

GLuint currentShader    ( int program );

void updateView       ( glm::mat4 view );
void updateProjection ( glm::mat4 projection );
void updateLight      ( glm::vec4 position , glm::vec4 ambient , glm::vec4 diffuse , glm::vec4 specular );


// orientation.cpp
void initOrientation ( );

void reorient    ( );
bool toggleLight ( bool enable );


// frame.cpp
void initFrame ( );
void drawFrame ( );
void drawTitle ( );


// IO.cpp
static void error    ( int error , const char * description );
static void resize   ( GLFWwindow * window , int width , int height );
static void keyboard ( GLFWwindow * window , int key , int scancode , int action , int mods );
static void click    ( GLFWwindow * window , int button , int action , int mods );
static void hover    ( GLFWwindow * window , double xpos , double ypos );
static void scroll   ( GLFWwindow * window , double xoffset , double yoffset );


#endif
