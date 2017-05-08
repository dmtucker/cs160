#define CTshader     shaders[shader_texture] //XXX
#define CTvPosition  shaderPositions[shader_texture] //XXX
#define CTvColor     shaderColors[shader_texture] //XXX
#define CTmodel      shaderModels[shader_texture] //XXX
#define CTview       shaderViews[shader_texture] //XXX
#define CTprojection shaderProjections[shader_texture] //XXX

GLuint
    CTtexture    = 0,
    CTvTexture   = 0;


void initShader_texture ( ) {
    static bool uninitialized = true;
    if (uninitialized) {

        shaders[shader_texture] = shaderProgram(
            "src/shaders/texture_vertex.glsl",
            "src/shaders/texture_fragment.glsl"
        );
        if (shaders[shader_texture] == 0) {
            fprintf(stderr,"Texture Shader Error\n");
            exit(EXIT_FAILURE);
        }
        currentShader(shaders[shader_texture]);

        shaderPositions[shader_texture]    = glGetAttribLocation( shaders[shader_texture],"position");
        shaderColors[shader_texture]       = glGetAttribLocation( shaders[shader_texture],"color");
        CTvTexture                         = glGetAttribLocation( shaders[shader_texture],"texture_coordinate");
        CTtexture                          = glGetUniformLocation(shaders[shader_texture],"texture_map");
        shaderModels[shader_texture]       = glGetUniformLocation(shaders[shader_texture],"model");
        shaderViews[shader_texture]        = glGetUniformLocation(shaders[shader_texture],"view");
        shaderProjections[shader_texture]  = glGetUniformLocation(shaders[shader_texture],"projection");

        glUniformMatrix4fv(shaderModels[shader_texture]     ,1,GL_FALSE,glm::value_ptr(glm::mat4()));
        glUniformMatrix4fv(shaderViews[shader_texture]      ,1,GL_FALSE,glm::value_ptr(glm::mat4()));
        glUniformMatrix4fv(shaderProjections[shader_texture],1,GL_FALSE,glm::value_ptr(glm::mat4()));
        glUniform1i(CTtexture,0);
    }
    else {
        currentShader(0);
        glDeleteProgram(shaders[shader_texture]);
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

    currentShader(shaders[shader_texture]);

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
    glEnableVertexAttribArray(shaderPositions[shader_texture]);
    glEnableVertexAttribArray(shaderColors[shader_texture]);
    glEnableVertexAttribArray(CTvTexture);
    glVertexAttribPointer(shaderPositions[shader_texture],4,GL_FLOAT,GL_FALSE,0,VOID(offset));      offset += size_points;
    glVertexAttribPointer(shaderColors[shader_texture]   ,4,GL_FLOAT,GL_FALSE,0,VOID(offset));      offset += size_colors;
    glVertexAttribPointer(CTvTexture ,2,GL_FLOAT,GL_FALSE,0,VOID(offset));      offset += size_textures;
}


void cleanCTobject ( GPUbuffer * VO ) {

    glBindVertexArray(0);
    glDeleteVertexArrays(1,&VO->VAO);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glDeleteBuffers(1,&VO->VBO);
}
