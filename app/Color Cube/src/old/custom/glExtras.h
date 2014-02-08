#ifndef GLEXTRAS_H
#define GLEXTRAS_H
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glew.h>


void glCheck ( const char * task ) {
    const GLenum status = glGetError();
    if (status != GL_NO_ERROR) {
        fprintf(stderr,"%s : %s\n",task,gluErrorString(status));
        exit(EXIT_FAILURE);
    }
}


GLuint glShader ( const char * source , GLenum type ) {
    GLuint shader = 0;
    
    size_t size = 0;
    struct stat info;
    if (stat(source,&info) == -1) {
        perror("unusable file");
        return -1;
    }
    else size = (size_t) info.st_size;
    
    FILE * file = fopen(source,"r");
    if (file == NULL) perror("unopenable file");
    else {
        char * GLSLfile = (char *) malloc(size+1);
        if (GLSLfile == NULL) fprintf(stderr,"Unavailable Memory: %ldB\n",size);
        else {
            if (fread((void *) GLSLfile,sizeof(char),size,file) == size) {
                GLSLfile[size] = '\0';
                shader = glCreateShader(type);
                if (shader == 0) glCheck("Shader Creation");
                else {
                    const char * GLSL = GLSLfile;
                    glShaderSource(shader,1,&GLSL,NULL);
                    glCompileShader(shader);
                    glCheck("Shader Compilation");
                }
                free(GLSLfile);
            }
            else fputs ("unreadable file",stderr);
        }
        fclose(file);
    }
    return shader;
}
#endif
