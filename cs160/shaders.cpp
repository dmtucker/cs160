#include "shader.hpp"

typedef struct {
    GLuint VBO;
    GLuint VAO;
} GPUbuffer;

#include "shaders/Cshader.cpp"
#include "shaders/CTshader.cpp"
#include "shaders/CLshader.cpp"
#include "shaders/CTLshader.cpp"


void initShaders ( ) {
    static bool uninitialized = true;
    if (uninitialized) {
        initCshader();
        initCTshader();
        initCLshader();
        initCTLshader();
    }
    else {
        initCTLshader();
        initCLshader();
        initCTshader();
        initCshader();
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


/*XXX Inelegant Solution */

void updateView ( glm::mat4 view ) {
    
    currentShader(Cshader);
    glUniformMatrix4fv(  Cview,1,GL_FALSE,glm::value_ptr(view));
    
    currentShader(CTshader);
    glUniformMatrix4fv( CTview,1,GL_FALSE,glm::value_ptr(view));
    
    currentShader(CLshader);
    glUniformMatrix4fv( CLview,1,GL_FALSE,glm::value_ptr(view));
    
    currentShader(CTLshader);
    glUniformMatrix4fv(CTLview,1,GL_FALSE,glm::value_ptr(view));
}


void updateProjection ( glm::mat4 projection ) {
    
    currentShader(Cshader);
    glUniformMatrix4fv(  Cprojection,1,GL_FALSE,glm::value_ptr(projection));
    
    currentShader(CTshader);
    glUniformMatrix4fv( CTprojection,1,GL_FALSE,glm::value_ptr(projection));
    
    currentShader(CLshader);
    glUniformMatrix4fv( CLprojection,1,GL_FALSE,glm::value_ptr(projection));
    
    currentShader(CTLshader);
    glUniformMatrix4fv(CTLprojection,1,GL_FALSE,glm::value_ptr(projection));
}


void updateLight ( glm::vec4 position , glm::vec4 ambient , glm::vec4 diffuse , glm::vec4 specular ) {
    
    currentShader(CLshader);
    glUniform4fv(CLlPosition ,1,glm::value_ptr(position));
    glUniform4fv(CLlAmbient  ,1,glm::value_ptr(ambient));
    glUniform4fv(CLlDiffuse  ,1,glm::value_ptr(diffuse));
    glUniform4fv(CLlSpecular ,1,glm::value_ptr(specular));
    
    currentShader(CTLshader);
    glUniform4fv(CTLlPosition,1,glm::value_ptr(position));
    glUniform4fv(CTLlAmbient ,1,glm::value_ptr(ambient));
    glUniform4fv(CTLlDiffuse ,1,glm::value_ptr(diffuse));
    glUniform4fv(CTLlSpecular,1,glm::value_ptr(specular));
}

/*XXX*/
