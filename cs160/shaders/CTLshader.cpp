GLuint
    CTLshader     = 0,
    CTLmodel      = 0,
    CTLview       = 0,
    CTLprojection = 0,
    CTLtexture    = 0,
    CTLvPosition  = 0,
    CTLvColor     = 0,
    CTLvTexture   = 0,
    CTLvNormal    = 0,
    CTLlPosition  = 0,
    CTLlAmbient   = 0,
    CTLlDiffuse   = 0,
    CTLlSpecular  = 0,
    CTLmShininess = 0,
    CTLmAmbient   = 0,
    CTLmDiffuse   = 0,
    CTLmSpecular  = 0;


void initCTLshader ( ) {
    static bool uninitialized = true;
    if (uninitialized) {
        
        CTLshader = shaderProgram(
            "src/shaders/vctl.glsl",
            "src/shaders/fctl.glsl"
        );
        if (CTLshader == 0) {
            fprintf(stderr,"CTLshader Error\n");
            exit(EXIT_FAILURE);
        }
        currentShader(CTLshader);
        
        CTLmodel       = glGetUniformLocation(CTLshader,"model");
        CTLview        = glGetUniformLocation(CTLshader,"view");
        CTLprojection  = glGetUniformLocation(CTLshader,"projection");
        CTLtexture     = glGetUniformLocation(CTLshader,"texture_map");
        CTLvPosition   = glGetAttribLocation( CTLshader,"position");
        CTLvColor      = glGetAttribLocation( CTLshader,"color");
        CTLvTexture    = glGetAttribLocation( CTLshader,"texture_coordinate");
        CTLvNormal     = glGetAttribLocation( CTLshader,"normal");
        CTLlPosition   = glGetUniformLocation(CTLshader,"light_position");
        CTLlAmbient    = glGetUniformLocation(CTLshader,"light_ambient");
        CTLlDiffuse    = glGetUniformLocation(CTLshader,"light_diffuse");
        CTLlSpecular   = glGetUniformLocation(CTLshader,"light_specular");
        CTLmShininess  = glGetUniformLocation(CTLshader,"material_shininess");
        CTLmAmbient    = glGetUniformLocation(CTLshader,"material_ambient");
        CTLmDiffuse    = glGetUniformLocation(CTLshader,"material_diffuse");
        CTLmSpecular   = glGetUniformLocation(CTLshader,"material_specular");
        
//        printf("CTLmodel       (%d)\n",CTLmodel);
//        printf("CTLview        (%d)\n",CTLview);
//        printf("CTLprojection  (%d)\n",CTLprojection);
//        printf("CTLtexture     (%d)\n",CTLtexture);
//        printf("CTLvPosition   (%d)\n",CTLvPosition);
//        printf("CTLvColor      (%d)\n",CTLvColor);
//        printf("CTLvTexture    (%d)\n",CTLvTexture);
//        printf("CTLvNormal     (%d)\n",CTLvNormal);
//        printf("CTLlPosition   (%d)\n",CTLlPosition);
//        printf("CTLlAmbient    (%d)\n",CTLlAmbient);
//        printf("CTLlDiffuse    (%d)\n",CTLlDiffuse);
//        printf("CTLlSpecular   (%d)\n",CTLlSpecular);
//        printf("CTLmShininess  (%d)\n",CTLmShininess);
//        printf("CTLmAmbient    (%d)\n",CTLmAmbient);
//        printf("CTLmDiffuse    (%d)\n",CTLmDiffuse);
//        printf("CTLmSpecular   (%d)\n",CTLmSpecular);
        
        glUniformMatrix4fv(CTLmodel     ,1,GL_FALSE,glm::value_ptr(glm::mat4()));
        glUniformMatrix4fv(CTLview      ,1,GL_FALSE,glm::value_ptr(glm::mat4()));
        glUniformMatrix4fv(CTLprojection,1,GL_FALSE,glm::value_ptr(glm::mat4()));
        glUniform1i(CTLtexture,0);
    }
    else {
        currentShader(0);
        glDeleteProgram(CTLshader);
    }
    uninitialized = not uninitialized;
}


void CTLobject ( GPUbuffer * VO , GLuint vertices , glm::vec4 *   points ,
                                                    glm::vec4 *   colors ,
                                                    glm::vec2 * textures ,
                                                    glm::vec4 *  normals ) {
    
    size_t offset = 0;
    size_t size_points   = sizeof(glm::vec4)*vertices;
    size_t size_colors   = sizeof(glm::vec4)*vertices;
    size_t size_textures = sizeof(glm::vec2)*vertices;
    size_t size_normals  = sizeof(glm::vec4)*vertices;
    
    currentShader(CTLshader);
    
    glGenBuffers(1,&VO->VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VO->VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        size_points+size_colors+size_textures+size_normals,
        NULL,
        GL_STATIC_DRAW
    );
    glBufferSubData(GL_ARRAY_BUFFER,offset,size_points  ,VOID(points));         offset += size_points;
    glBufferSubData(GL_ARRAY_BUFFER,offset,size_colors  ,VOID(colors));         offset += size_colors;
    glBufferSubData(GL_ARRAY_BUFFER,offset,size_textures,VOID(textures));       offset += size_textures;
    glBufferSubData(GL_ARRAY_BUFFER,offset,size_normals ,VOID(normals));        offset += size_normals;
    
    offset = 0;
    
    glGenVertexArrays(1,&VO->VAO);
    glBindVertexArray(VO->VAO);
    glEnableVertexAttribArray(CTLvPosition);
    glEnableVertexAttribArray(CTLvColor);
    glEnableVertexAttribArray(CTLvTexture);
    glEnableVertexAttribArray(CTLvNormal);
    glVertexAttribPointer(CTLvPosition,4,GL_FLOAT,GL_FALSE,0,VOID(offset));     offset += size_points;
    glVertexAttribPointer(CTLvColor   ,4,GL_FLOAT,GL_FALSE,0,VOID(offset));     offset += size_colors;
    glVertexAttribPointer(CTLvTexture ,2,GL_FLOAT,GL_FALSE,0,VOID(offset));     offset += size_textures;
    glVertexAttribPointer(CTLvNormal  ,4,GL_FLOAT,GL_FALSE,0,VOID(offset));     offset += size_normals;
}


void cleanCTLobject ( GPUbuffer * VO ) {
    
    glBindVertexArray(0);
    glDeleteVertexArrays(1,&VO->VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glDeleteBuffers(1,&VO->VBO);
}
