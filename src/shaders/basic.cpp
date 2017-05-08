#define Cshader     shaders[shader_basic] //XXX
#define CvPosition  shaderPositions[shader_basic] //XXX
#define CvColor     shaderColors[shader_basic] //XXX
#define Cmodel      shaderModels[shader_basic] //XXX
#define Cview       shaderViews[shader_basic] //XXX
#define Cprojection shaderProjections[shader_basic] //XXX


void initShader_basic ( ) {
    static bool uninitialized = true;
    if (uninitialized) {

        shaders[shader_basic] = shaderProgram(
            "src/shaders/basic_vertex.glsl",
            "src/shaders/basic_fragment.glsl"
        );
        if (shaders[shader_basic] == 0) {
            fprintf(stderr,"Basic Shader Error\n");
            exit(EXIT_FAILURE);
        }
        currentShader(shaders[shader_basic]);

        shaderPositions[shader_basic]   = glGetAttribLocation( shaders[shader_basic],"position");
        shaderColors[shader_basic]      = glGetAttribLocation( shaders[shader_basic],"color");
        shaderModels[shader_basic]      = glGetUniformLocation(shaders[shader_basic],"model");
        shaderViews[shader_basic]       = glGetUniformLocation(shaders[shader_basic],"view");
        shaderProjections[shader_basic] = glGetUniformLocation(shaders[shader_basic],"projection");

        glUniformMatrix4fv(shaderModels[shader_basic]     ,1,GL_FALSE,glm::value_ptr(glm::mat4()));
        glUniformMatrix4fv(shaderViews[shader_basic]      ,1,GL_FALSE,glm::value_ptr(glm::mat4()));
        glUniformMatrix4fv(shaderProjections[shader_basic],1,GL_FALSE,glm::value_ptr(glm::mat4()));
    }
    else {
        currentShader(0);
        glDeleteProgram(shaders[shader_basic]);
    }
    uninitialized = not uninitialized;
}


void Cobject ( GPUbuffer * VO , GLuint vertices , glm::vec4 * points ,
                                                  glm::vec4 * colors ) {

    size_t offset = 0;
    size_t size_points = sizeof(glm::vec4)*vertices;
    size_t size_colors = sizeof(glm::vec4)*vertices;

    currentShader(shaders[shader_basic]);

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
    glEnableVertexAttribArray(shaderPositions[shader_basic]);
    glEnableVertexAttribArray(shaderColors[shader_basic]);
    glVertexAttribPointer(shaderPositions[shader_basic],4,GL_FLOAT,GL_FALSE,0,VOID(offset));       offset += size_points;
    glVertexAttribPointer(shaderColors[shader_basic]   ,4,GL_FLOAT,GL_FALSE,0,VOID(offset));       offset += size_colors;
}


void cleanCobject ( GPUbuffer * VO ) {

    glBindVertexArray(0);
    glDeleteVertexArrays(1,&VO->VAO);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glDeleteBuffers(1,&VO->VBO);
}
