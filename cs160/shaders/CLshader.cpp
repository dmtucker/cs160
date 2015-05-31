GLuint
    CLshader     = 0,
    CLmodel      = 0,
    CLview       = 0,
    CLprojection = 0,
    CLvPosition  = 0,
    CLvColor     = 0,
    CLvNormal    = 0,
    CLlPosition  = 0,
    CLlAmbient   = 0,
    CLlDiffuse   = 0,
    CLlSpecular  = 0,
    CLmShininess = 0,
    CLmAmbient   = 0,
    CLmDiffuse   = 0,
    CLmSpecular  = 0;


void initCLshader ( ) {
    static bool uninitialized = true;
    if (uninitialized) {
        
        CLshader = shaderProgram(
            "src/shaders/vcl.glsl",
            "src/shaders/fcl.glsl"
        );
        if (CLshader == 0) {
            fprintf(stderr,"CLshader Error\n");
            exit(EXIT_FAILURE);
        }
        currentShader(CLshader);
        
        CLmodel       = glGetUniformLocation(CLshader,"model");
        CLview        = glGetUniformLocation(CLshader,"view");
        CLprojection  = glGetUniformLocation(CLshader,"projection");
        CLvPosition   = glGetAttribLocation( CLshader,"position");
        CLvColor      = glGetAttribLocation( CLshader,"color");
        CLvNormal     = glGetAttribLocation( CLshader,"normal");
        CLlPosition   = glGetUniformLocation(CLshader,"light_position");
        CLlAmbient    = glGetUniformLocation(CLshader,"light_ambient");
        CLlDiffuse    = glGetUniformLocation(CLshader,"light_diffuse");
        CLlSpecular   = glGetUniformLocation(CLshader,"light_specular");
        CLmShininess  = glGetUniformLocation(CLshader,"material_shininess");
        CLmAmbient    = glGetUniformLocation(CLshader,"material_ambient");
        CLmDiffuse    = glGetUniformLocation(CLshader,"material_diffuse");
        CLmSpecular   = glGetUniformLocation(CLshader,"material_specular");
        
//        printf("CLmodel       (%d)\n",CLmodel);
//        printf("CLview        (%d)\n",CLview);
//        printf("CLprojection  (%d)\n",CLprojection);
//        printf("CLvPosition   (%d)\n",CLvPosition);
//        printf("CLvColor      (%d)\n",CLvColor);
//        printf("CLvNormal     (%d)\n",CLvNormal);
//        printf("CLlPosition   (%d)\n",CLlPosition);
//        printf("CLlAmbient    (%d)\n",CLlAmbient);
//        printf("CLlDiffuse    (%d)\n",CLlDiffuse);
//        printf("CLlSpecular   (%d)\n",CLlSpecular);
//        printf("CLmShininess  (%d)\n",CLmShininess);
//        printf("CLmAmbient    (%d)\n",CLmAmbient);
//        printf("CLmDiffuse    (%d)\n",CLmDiffuse);
//        printf("CLmSpecular   (%d)\n",CLmSpecular);
        
        glUniformMatrix4fv(CLmodel     ,1,GL_FALSE,glm::value_ptr(glm::mat4()));
        glUniformMatrix4fv(CLview      ,1,GL_FALSE,glm::value_ptr(glm::mat4()));
        glUniformMatrix4fv(CLprojection,1,GL_FALSE,glm::value_ptr(glm::mat4()));
    }
    else {
        currentShader(0);
        glDeleteProgram(CLshader);
    }
    uninitialized = not uninitialized;
}


void CLobject ( GPUbuffer * VO , GLuint vertices , glm::vec4 *  points ,
                                                   glm::vec4 *  colors ,
                                                   glm::vec4 * normals ) {
    
    size_t offset = 0;
    size_t size_points  = sizeof(glm::vec4)*vertices;
    size_t size_colors  = sizeof(glm::vec4)*vertices;
    size_t size_normals = sizeof(glm::vec4)*vertices;
    
    currentShader(CLshader);
    
    glGenBuffers(1,&VO->VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VO->VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        size_points+size_colors+size_normals,
        NULL,
        GL_STATIC_DRAW
    );
    glBufferSubData(GL_ARRAY_BUFFER,offset,size_points ,VOID(points));          offset += size_points;
    glBufferSubData(GL_ARRAY_BUFFER,offset,size_colors ,VOID(colors));          offset += size_colors;
    glBufferSubData(GL_ARRAY_BUFFER,offset,size_normals,VOID(normals));         offset += size_normals;
    
    offset = 0;
    
    glGenVertexArrays(1,&VO->VAO);
    glBindVertexArray(VO->VAO);
    glEnableVertexAttribArray(CLvPosition);
    glEnableVertexAttribArray(CLvColor);
    glEnableVertexAttribArray(CLvNormal);
    glVertexAttribPointer(CLvPosition,4,GL_FLOAT,GL_FALSE,0,VOID(offset));      offset += size_points;
    glVertexAttribPointer(CLvColor   ,4,GL_FLOAT,GL_FALSE,0,VOID(offset));      offset += size_colors;
    glVertexAttribPointer(CLvNormal  ,4,GL_FLOAT,GL_FALSE,0,VOID(offset));      offset += size_normals;
}


void cleanCLobject ( GPUbuffer * VO ) {
    
    glBindVertexArray(0);
    glDeleteVertexArrays(1,&VO->VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glDeleteBuffers(1,&VO->VBO);
}
