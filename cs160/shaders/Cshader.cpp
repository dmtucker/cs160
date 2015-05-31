GLuint
    Cshader     = 0,
    Cmodel      = 0,
    Cview       = 0,
    Cprojection = 0,
    CvPosition  = 0,
    CvColor     = 0;


void initCshader ( ) {
    static bool uninitialized = true;
    if (uninitialized) {
        
        Cshader = shaderProgram(
            "src/shaders/vc.glsl",
            "src/shaders/fc.glsl"
        );
        if (Cshader == 0) {
            fprintf(stderr,"Cshader Error\n");
            exit(EXIT_FAILURE);
        }
        currentShader(Cshader);
        
        Cmodel      = glGetUniformLocation(Cshader,"model");
        Cview       = glGetUniformLocation(Cshader,"view");
        Cprojection = glGetUniformLocation(Cshader,"projection");
        CvPosition  = glGetAttribLocation( Cshader,"position");
        CvColor     = glGetAttribLocation( Cshader,"color");
        
//        printf("Cmodel      (%d)\n",Cmodel);
//        printf("Cview       (%d)\n",Cview);
//        printf("Cprojection (%d)\n",Cprojection);
//        printf("CvPosition  (%d)\n",CvPosition);
//        printf("CvColor     (%d)\n",CvColor);
        
        glUniformMatrix4fv(Cmodel     ,1,GL_FALSE,glm::value_ptr(glm::mat4()));
        glUniformMatrix4fv(Cview      ,1,GL_FALSE,glm::value_ptr(glm::mat4()));
        glUniformMatrix4fv(Cprojection,1,GL_FALSE,glm::value_ptr(glm::mat4()));
    }
    else {
        currentShader(0);
        glDeleteProgram(Cshader);
    }
    uninitialized = not uninitialized;
}


void Cobject ( GPUbuffer * VO , GLuint vertices , glm::vec4 * points ,
                                                  glm::vec4 * colors ) {
    
    size_t offset = 0;
    size_t size_points = sizeof(glm::vec4)*vertices;
    size_t size_colors = sizeof(glm::vec4)*vertices;
    
    currentShader(Cshader);
    
    glGenBuffers(1,&VO->VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VO->VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        size_points+size_colors,
        NULL,
        GL_STATIC_DRAW
    );
    glBufferSubData(GL_ARRAY_BUFFER,offset,size_points,VOID(points));           offset += size_points;
    glBufferSubData(GL_ARRAY_BUFFER,offset,size_colors,VOID(colors));           offset += size_colors;
    
    offset = 0;
    
    glGenVertexArrays(1,&VO->VAO);
    glBindVertexArray(VO->VAO);
    glEnableVertexAttribArray(CvPosition);
    glEnableVertexAttribArray(CvColor);
    glVertexAttribPointer(CvPosition,4,GL_FLOAT,GL_FALSE,0,VOID(offset));       offset += size_points;
    glVertexAttribPointer(CvColor   ,4,GL_FLOAT,GL_FALSE,0,VOID(offset));       offset += size_colors;
}


void cleanCobject ( GPUbuffer * VO ) {
    
    glBindVertexArray(0);
    glDeleteVertexArrays(1,&VO->VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glDeleteBuffers(1,&VO->VBO);
}
