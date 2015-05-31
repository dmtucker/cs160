#include "shader.hpp"

typedef struct {
    GLuint VBO;
    GLuint VAO;
} GPUbuffer;

#define   SHADERS 4

GLuint
    shaders[SHADERS],
    shader_basic   = 0,
    shader_texture = 1,
    shader_light   = 2,
    shader_blt     = 3,
    
    shaderPositions[SHADERS],
    shaderColors[SHADERS],
    shaderModels[SHADERS],
    shaderViews[SHADERS],
    shaderProjections[SHADERS];

#include "shaders/basic.cpp"
#include "shaders/texture.cpp"
#include "shaders/light.cpp"
#include "shaders/blt.cpp"


void initShaders ( ) {
    static bool uninitialized = true;
    if (uninitialized) {
        initShader_basic();
        initShader_texture();
        initShader_light();
        initShader_blt();
    }
    else {
        initShader_blt();
        initShader_light();
        initShader_texture();
        initShader_basic();
    }
    uninitialized = not uninitialized;
}


GLuint currentShader ( int program = -1 ) {
    static GLuint current = 0;
    if (program > -1) {
        current = program;
        glUseProgram(current);
    }
    return current;
}


void updateView ( glm::mat4 view ) {
    for (int i=0; i < SHADERS ;++i) {
        currentShader(shaders[i]);
        glUniformMatrix4fv(shaderViews[i],1,GL_FALSE,glm::value_ptr(view));
    }
}


void updateProjection ( glm::mat4 projection ) {
    for (int i=0; i < SHADERS ;++i) {
        currentShader(shaders[i]);
        glUniformMatrix4fv(shaderProjections[i],1,GL_FALSE,glm::value_ptr(projection));
    }
}


void updateLight ( glm::vec4 position , glm::vec4 ambient , glm::vec4 diffuse , glm::vec4 specular ) {
    
    currentShader(shaders[shader_light]);
    glUniform4fv(CLlPosition ,1,glm::value_ptr(position));
    glUniform4fv(CLlAmbient  ,1,glm::value_ptr(ambient));
    glUniform4fv(CLlDiffuse  ,1,glm::value_ptr(diffuse));
    glUniform4fv(CLlSpecular ,1,glm::value_ptr(specular));
    
    currentShader(shaders[shader_blt]);
    glUniform4fv(CTLlPosition,1,glm::value_ptr(position));
    glUniform4fv(CTLlAmbient ,1,glm::value_ptr(ambient));
    glUniform4fv(CTLlDiffuse ,1,glm::value_ptr(diffuse));
    glUniform4fv(CTLlSpecular,1,glm::value_ptr(specular));
}
