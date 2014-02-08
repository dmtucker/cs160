#ifndef SHADER_HPP
#define SHADER_HPP

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <GL/glew.h>


bool checkGL ( const char * file , const unsigned int line , const bool required = false ) {
    const GLenum status = glGetError();
    if (status != GL_NO_ERROR) {
        fprintf(stderr,"%s:%d %s\n",gluErrorString(status));
        if (required) exit(EXIT_FAILURE);
        return false;
    }
    return true;
}


bool checkShaderCompilation ( GLuint shader ) {
    GLint compiled;
    glGetShaderiv(shader,GL_COMPILE_STATUS,&compiled);
    if (not compiled) {
        GLint logSize;
        glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&logSize);
        
        char * buffer = new char[logSize];
        glGetShaderInfoLog(shader,logSize,NULL,buffer);
        fprintf(stderr,"%s\n",buffer);
        delete [] buffer;
        return false;
    }
    return true;
}


bool initShader ( GLuint * shader , const char * source , GLenum type ) {
    size_t size = 0;
    struct stat info;
    if (stat(source,&info) == -1) {
        perror("Unsizable Shader File");
        return false;
    }
    else size = (size_t) info.st_size;
    
    FILE * file = fopen(source,"r");
    if (file == NULL) {
        perror("Unopenable Shader File");
        return false;
    }
    
    bool ok = true;
    
    char * GLSLfile = (char *) malloc(size+1);
    if (GLSLfile == NULL) {
        fprintf(stderr,"Memory Unavailable (%ldB)\n",size);
        ok = false;
    }
    else {
        if (fread((void *) GLSLfile,sizeof(char),size,file) == size) {
            GLSLfile[size] = '\0';
            *shader = glCreateShader(type);
            if (*shader == 0) {
                fputs("Shader Creation Error\n",stderr);
                checkGL(__FILE__,__LINE__);
                ok = false;
            }
            else {
                const char * GLSL = GLSLfile;
                glShaderSource(*shader,1,&GLSL,NULL);
                checkGL(__FILE__,__LINE__);
                glCompileShader(*shader);
                if (not checkShaderCompilation(*shader)) {
                    fputs("Shader Compilation Error\n",stderr);
                    ok = false;
                }
            }
        }
        else {
            fputs("Corrupt Shader File\n",stderr);
            ok = false;
        }
        free(GLSLfile);
    }
    
    if (fclose(file) == EOF) fputs("Unclosable Shader File\n",stderr);
    return ok;
}


bool checkShaderLinkage ( GLuint program ) {
    GLint linked;
    glGetProgramiv(program,GL_LINK_STATUS,&linked);
    if (not linked) {
        GLint logSize;
        glGetProgramiv(program,GL_INFO_LOG_LENGTH,&logSize);
        
        char * buffer = new char[logSize];
        glGetProgramInfoLog(program,logSize,NULL,buffer);
        fprintf(stderr,"%s\n",buffer);
        delete [] buffer;
        return false;
    }
    return true;
}


GLuint initShaders ( const char * vShader , const char * fShader ) {
    GLuint program = glCreateProgram();
    checkGL(__FILE__,__LINE__,true);
    
    GLuint shader = 0;
    
    if (initShader(&shader,vShader,GL_VERTEX_SHADER)) glAttachShader(program,shader);
    else fputs("Vertex Shader Initialization Error\n",stderr);
    checkGL(__FILE__,__LINE__,true);
    
    
    if (initShader(&shader,fShader,GL_FRAGMENT_SHADER)) glAttachShader(program,shader);
    else fputs("Fragment Shader Initialization Error\n",stderr);
    checkGL(__FILE__,__LINE__,true);
    
    glLinkProgram(program);
    if (not checkShaderLinkage(program)) {
        fputs("Shader Program Linkage Error\n",stderr);
        exit(EXIT_FAILURE);
    }
    
    return program;
}


#endif
