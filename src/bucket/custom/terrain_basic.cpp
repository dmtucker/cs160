GLuint terrainVAO   = 0;
mat4   terrainModel = mat4();


void initTerrain ( ) {
    
    GLuint terrainVBO = 0;
    
    static bool uninitialized = true;
    if (uninitialized) {
        
        GLuint p = 0;
        vec4 points[terrain.width*terrain.height];
        for (int i=0; i < terrain.height ;++i)
            for (int j=0; j < terrain.width ;++j)
                points[p++] = vec4(j,RED(PIXEL(terrain,i,j)),i,1);
        
        currentShader(basicShader);
        
        glGenBuffers(1,&terrainVBO);
        glBindBuffer(GL_ARRAY_BUFFER,terrainVBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(points),points,GL_STATIC_DRAW);
        checkGL(__FILE__,__LINE__);
        
        glGenVertexArrays(1,&terrainVAO);
        glBindVertexArray(terrainVAO);
        glEnableVertexAttribArray(BSVposition);
        glVertexAttribPointer(BSVposition,4,GL_FLOAT,GL_FALSE,0,VOID(0));
        checkGL(__FILE__,__LINE__);
        
        uninitialized = false;
    }
    else {
        
        glBindVertexArray(0);
        glDeleteVertexArrays(1,&terrainVAO);
        glDeleteBuffers(1,&terrainVBO);
        checkGL(__FILE__,__LINE__);
        
        uninitialized = true;
    }
}


void drawTerrain ( ) {
    currentShader(basicShader);
    glBindVertexArray(terrainVAO);
    glUniformMatrix4fv(BSmodel,1,GL_FALSE,value_ptr(terrainModel));
    glDrawArrays(GL_POINTS,0,terrain.width*terrain.height);
    checkGL(__FILE__,__LINE__);
}
