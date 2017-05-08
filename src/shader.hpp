#ifndef SHADER_HPP
#define SHADER_HPP


#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <GL/glew.h>


GLint checkShaderCompilation ( GLuint shader ) {
    GLint compiled;
    glGetShaderiv(shader,GL_COMPILE_STATUS,&compiled);
    if (not compiled) {

        GLint logSize;
        glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&logSize);

        char * buffer = new char[logSize];
        glGetShaderInfoLog(shader,logSize,NULL,buffer);
        fprintf(stderr,"%s\n",buffer);
        delete [] buffer;
    }
    return compiled;
}


GLuint shader ( GLenum type , const char * source ) {

    GLuint shader = 0;

    struct stat info;
    if (stat(source,&info) == -1) {
        perror("Unsizable Shader File");
        return 0;
    }

    FILE * file = fopen(source,"r");
    if (file == NULL) {
        perror("Unreadable Shader File");
        return 0;
    }

    char * GLSLfile = (char *) malloc(info.st_size+1);
    if (GLSLfile == NULL) fprintf(stderr,"Memory Unavailable (%ldB)\n",info.st_size);
    else {

        if (fread((void *) GLSLfile,sizeof(char),info.st_size,file) == info.st_size) {
            GLSLfile[info.st_size] = '\0';

            shader = glCreateShader(type);
            if (shader == 0) fprintf(stderr,"Shader Compilation Error (%s)\n",source);
            else {
                const char * GLSL = GLSLfile;
                glShaderSource(shader,1,&GLSL,NULL);
                glCompileShader(shader);
                if (not checkShaderCompilation(shader)) {
                    fprintf(stderr,"Shader Compilation Error (%s)\n",source);
                    shader = 0;
                }
            }
        }
        else fprintf(stderr,"Corrupt Shader File (%s)\n",source);

        free(GLSLfile);
    }

    if (fclose(file) == EOF) fprintf(stderr,"Unclosable Shader File (%s)\n",source);

    return shader;
}


GLint checkShaderProgramLinkage ( GLuint program ) {
    GLint linked;
    glGetProgramiv(program,GL_LINK_STATUS,&linked);
    if (not linked) {

        GLint logSize;
        glGetProgramiv(program,GL_INFO_LOG_LENGTH,&logSize);

        char * buffer = new char[logSize];
        glGetProgramInfoLog(program,logSize,NULL,buffer);
        fprintf(stderr,"%s\n",buffer);
        delete [] buffer;
    }
    return linked;
}


GLuint shaderProgram ( const char * vertexShader , const char * fragmentShader ) {

    GLuint program = glCreateProgram();
    if (program == 0) {
        fprintf(stderr,"Shader Program Creation Error");
        return 0;
    }

    GLuint vertex = shader(GL_VERTEX_SHADER,vertexShader);
    if (vertex == 0) {
        fprintf(stderr,"Vertex Shader Error (%s)\n",vertexShader);
        return 0;
    }

    GLuint fragment = shader(GL_FRAGMENT_SHADER,fragmentShader);
    if (fragment == 0) {
        fprintf(stderr,"Fragment Shader Error (%s)\n",fragmentShader);
        return 0;
    }

    glAttachShader(program,vertex);
    glAttachShader(program,fragment);

    glLinkProgram(program);
    if (not checkShaderProgramLinkage(program)) {
        fprintf(stderr,"Shader Program Linkage Error\n");
        return 0;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}


#endif
