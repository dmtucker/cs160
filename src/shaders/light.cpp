#define CLshader     shaders[shader_texture] //XXX
#define CLvPosition  shaderPositions[shader_texture] //XXX
#define CLvColor     shaderColors[shader_texture] //XXX
#define CLmodel      shaderModels[shader_texture] //XXX
#define CLview       shaderViews[shader_texture] //XXX
#define CLprojection shaderProjections[shader_texture] //XXX

GLuint
    CLvNormal    = 0,
    CLlPosition  = 0,
    CLlAmbient   = 0,
    CLlDiffuse   = 0,
    CLlSpecular  = 0,
    CLmShininess = 0,
    CLmAmbient   = 0,
    CLmDiffuse   = 0,
    CLmSpecular  = 0;


void initShader_light ( ) {
    static bool uninitialized = true;
    if (uninitialized) {
        
        shaders[shader_light] = shaderProgram(
            "src/shaders/vcl.glsl",
            "src/shaders/fcl.glsl"
        );
        if (shaders[shader_light] == 0) {
            fprintf(stderr,"Light Shader Error\n");
            exit(EXIT_FAILURE);
        }
        currentShader(shaders[shader_light]);
        
        shaderPositions[shader_light]   = glGetAttribLocation( shaders[shader_light],"position");
        shaderColors[shader_light]      = glGetAttribLocation( shaders[shader_light],"color");
        CLvNormal     = glGetAttribLocation( shaders[shader_light],"normal");
        CLlPosition   = glGetUniformLocation(shaders[shader_light],"light_position");
        CLlAmbient    = glGetUniformLocation(shaders[shader_light],"light_ambient");
        CLlDiffuse    = glGetUniformLocation(shaders[shader_light],"light_diffuse");
        CLlSpecular   = glGetUniformLocation(shaders[shader_light],"light_specular");
        CLmShininess  = glGetUniformLocation(shaders[shader_light],"material_shininess");
        CLmAmbient    = glGetUniformLocation(shaders[shader_light],"material_ambient");
        CLmDiffuse    = glGetUniformLocation(shaders[shader_light],"material_diffuse");
        CLmSpecular   = glGetUniformLocation(shaders[shader_light],"material_specular");
        shaderModels[shader_light]       = glGetUniformLocation(shaders[shader_light],"model");
        shaderViews[shader_light]        = glGetUniformLocation(shaders[shader_light],"view");
        shaderProjections[shader_light]  = glGetUniformLocation(shaders[shader_light],"projection");
        
        glUniformMatrix4fv(shaderModels[shader_light]     ,1,GL_FALSE,glm::value_ptr(glm::mat4()));
        glUniformMatrix4fv(shaderViews[shader_light]      ,1,GL_FALSE,glm::value_ptr(glm::mat4()));
        glUniformMatrix4fv(shaderProjections[shader_light],1,GL_FALSE,glm::value_ptr(glm::mat4()));
    }
    else {
        currentShader(0);
        glDeleteProgram(shaders[shader_light]);
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
    
    currentShader(shaders[shader_light]);
    
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
    glEnableVertexAttribArray(shaderPositions[shader_light]);
    glEnableVertexAttribArray(shaderColors[shader_light]);
    glEnableVertexAttribArray(CLvNormal);
    glVertexAttribPointer(shaderPositions[shader_light],4,GL_FLOAT,GL_FALSE,0,VOID(offset));      offset += size_points;
    glVertexAttribPointer(shaderColors[shader_light]   ,4,GL_FLOAT,GL_FALSE,0,VOID(offset));      offset += size_colors;
    glVertexAttribPointer(CLvNormal  ,4,GL_FLOAT,GL_FALSE,0,VOID(offset));      offset += size_normals;
}


void cleanCLobject ( GPUbuffer * VO ) {
    
    glBindVertexArray(0);
    glDeleteVertexArrays(1,&VO->VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glDeleteBuffers(1,&VO->VBO);
}
