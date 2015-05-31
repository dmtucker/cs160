GLuint
    CTshader     = 0,
    CTmodel      = 0,
    CTview       = 0,
    CTprojection = 0,
    CTtexture    = 0,
    CTvPosition  = 0,
    CTvColor     = 0,
    CTvTexture   = 0;


void initCTshader ( ) {
    static bool uninitialized = true;
    if (uninitialized) {
        
        CTshader = shaderProgram(
            "src/shaders/vct.glsl",
            "src/shaders/fct.glsl"
        );
        if (CTshader == 0) {
            fprintf(stderr,"CTshader Error\n");
            exit(EXIT_FAILURE);
        }
        currentShader(CTshader);
        
        CTmodel       = glGetUniformLocation(CTshader,"model");
        CTview        = glGetUniformLocation(CTshader,"view");
        CTprojection  = glGetUniformLocation(CTshader,"projection");
        CTtexture     = glGetUniformLocation(CTshader,"texture_map");
        CTvPosition   = glGetAttribLocation( CTshader,"position");
        CTvColor      = glGetAttribLocation( CTshader,"color");
        CTvTexture    = glGetAttribLocation( CTshader,"texture_coordinate");
        
//        printf("CTmodel       (%d)\n",CTmodel);
//        printf("CTview        (%d)\n",CTview);
//        printf("CTprojection  (%d)\n",CTprojection);
//        printf("CTtexture     (%d)\n",CTtexture);
//        printf("CTvPosition   (%d)\n",CTvPosition);
//        printf("CTvColor      (%d)\n",CTvColor);
//        printf("CTvTexture    (%d)\n",CTvTexture);
        
        glUniformMatrix4fv(CTmodel     ,1,GL_FALSE,glm::value_ptr(glm::mat4()));
        glUniformMatrix4fv(CTview      ,1,GL_FALSE,glm::value_ptr(glm::mat4()));
        glUniformMatrix4fv(CTprojection,1,GL_FALSE,glm::value_ptr(glm::mat4()));
        glUniform1i(CTtexture,0);
    }
    else {
        currentShader(0);
        glDeleteProgram(CTshader);
    }
    uninitialized = not uninitialized;
}


void CTobject ( GPUbuffer * VO , GLuint vertices , glm::vec4 * points ,
                                                   glm::vec4 * colors ,
                                                   glm::vec2 * textures ) {
    
    size_t offset = 0;
    size_t size_points   = sizeof(glm::vec4)*vertices;
    size_t size_colors   = sizeof(glm::vec4)*vertices;
    size_t size_textures = sizeof(glm::vec2)*vertices;
    
    currentShader(Cshader);
    
    glGenBuffers(1,&VO->VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VO->VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        size_points+size_colors+size_textures,
        NULL,
        GL_STATIC_DRAW
    );
    glBufferSubData(GL_ARRAY_BUFFER,offset,size_points  ,VOID(points));         offset += size_points;
    glBufferSubData(GL_ARRAY_BUFFER,offset,size_colors  ,VOID(colors));         offset += size_colors;
    glBufferSubData(GL_ARRAY_BUFFER,offset,size_textures,VOID(textures));       offset += size_textures;
    
    offset = 0;
    
    glGenVertexArrays(1,&VO->VAO);
    glBindVertexArray(VO->VAO);
    glEnableVertexAttribArray(CTvPosition);
    glEnableVertexAttribArray(CTvColor);
    glEnableVertexAttribArray(CTvTexture);
    glVertexAttribPointer(CTvPosition,4,GL_FLOAT,GL_FALSE,0,VOID(offset));      offset += size_points;
    glVertexAttribPointer(CTvColor   ,4,GL_FLOAT,GL_FALSE,0,VOID(offset));      offset += size_colors;
    glVertexAttribPointer(CTvTexture ,2,GL_FLOAT,GL_FALSE,0,VOID(offset));      offset += size_textures;
}


void cleanCTobject ( GPUbuffer * VO ) {
    
    glBindVertexArray(0);
    glDeleteVertexArrays(1,&VO->VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glDeleteBuffers(1,&VO->VBO);
}
