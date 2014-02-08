const unsigned int colorCubeTriangles = 12;
GLuint colorCubeVAO = 0, colorCubeVBO = 0;
mat4 colorCubeModel = mat4();


void initColorCube ( ) {
    static bool uninitialized = true;
    if (uninitialized) {
        
        const vec4 vCube[8] = {
            vec4(0,0,0,1), // left  lower back
            vec4(0,0,1,1), // left  lower front
            vec4(0,1,0,1), // left  upper back
            vec4(0,1,1,1), // left  upper front
            vec4(1,0,0,1), // right lower back
            vec4(1,0,1,1), // right lower front
            vec4(1,1,0,1), // right upper back
            vec4(1,1,1,1)  // right upper front
        };
        const vec4 cCube[8] = {
            palette[BASIC_BLACK],
            palette[BASIC_BLUE],
            palette[BASIC_GREEN],
            palette[BASIC_CYAN],
            palette[BASIC_RED],
            palette[BASIC_MAGENTA],
            palette[BASIC_YELLOW],
            palette[BASIC_WHITE]
        };
        const unsigned int iCube[6][4] = {
            {0,2,6,4}, // back
            {1,0,4,5}, // bottom
            {2,0,1,3}, // left
            {3,7,6,2}, // top
            {4,6,7,5}, // right
            {5,7,3,1}  // front
        };
        
        vec4 points[colorCubeTriangles*3], colors[colorCubeTriangles*3];
        unsigned int p = 0;
        for (int i = 0; i < 6 ;++i) {
            points[p] = vCube[iCube[i][0]];
            colors[p] = cCube[iCube[i][0]];
            ++p;
            
            points[p] = vCube[iCube[i][1]];
            colors[p] = cCube[iCube[i][1]];
            ++p;
            
            points[p] = vCube[iCube[i][2]];
            colors[p] = cCube[iCube[i][2]];
            ++p;
            
            points[p] = vCube[iCube[i][0]];
            colors[p] = cCube[iCube[i][0]];
            ++p;
            
            points[p] = vCube[iCube[i][2]];
            colors[p] = cCube[iCube[i][2]];
            ++p;
            
            points[p] = vCube[iCube[i][3]];
            colors[p] = cCube[iCube[i][3]];
            ++p;
        }
        
        glGenBuffers(1,&colorCubeVBO);
        glBindBuffer(GL_ARRAY_BUFFER,colorCubeVBO);
        glBufferData(   GL_ARRAY_BUFFER,sizeof(points)+sizeof(colors),NULL,GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER,0             ,sizeof(points),points);
        glBufferSubData(GL_ARRAY_BUFFER,sizeof(points),sizeof(colors),colors);
        checkGL(__FILE__,__LINE__);
        
        glGenVertexArrays(1,&colorCubeVAO);
        glBindVertexArray(colorCubeVAO);
        glEnableVertexAttribArray(shaderPosition);
        glEnableVertexAttribArray(shaderColor);
        glVertexAttribPointer(shaderPosition,4,GL_FLOAT,GL_FALSE,0,VOID(0));
        glVertexAttribPointer(shaderColor   ,4,GL_FLOAT,GL_FALSE,0,VOID(sizeof(points)));
        checkGL(__FILE__,__LINE__);
        
        uninitialized = false;
    }
    else {
        
        glBindVertexArray(0);
        glDeleteVertexArrays(1,&colorCubeVAO);
        glDeleteBuffers(1,&colorCubeVBO);
        checkGL(__FILE__,__LINE__);
        
        uninitialized = true;
    }
}


void drawColorCube ( ) {
    glBindVertexArray(colorCubeVAO);
    glUniformMatrix4fv(shaderModel,1,GL_FALSE,value_ptr(colorCubeModel));
    glDrawArrays(GL_TRIANGLES,0,colorCubeTriangles*3);
    checkGL(__FILE__,__LINE__);
}
