//XXX Deploy IBOs!

GLuint RGBcubeVAO = 0, BGRcubeVAO = 0;


void initRGBcube ( ) {
    
    static GLuint RGBcubeVBO = 0;
    
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
        const unsigned int iCube[6][4] = {
            {0,2,6,4}, // back
            {1,0,4,5}, // bottom
            {2,0,1,3}, // left
            {3,7,6,2}, // top
            {4,6,7,5}, // right
            {5,7,3,1}  // front
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
        
        vec4 points[36], colors[36];
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
        
        currentShader(Cshader);
        
        size_t offset = 0;
        glGenBuffers(1,&RGBcubeVBO);
        glBindBuffer(GL_ARRAY_BUFFER,RGBcubeVBO);
        glBufferData(GL_ARRAY_BUFFER,
            sizeof(points)+sizeof(colors),
            NULL,
            GL_STATIC_DRAW
        );
        glBufferSubData(GL_ARRAY_BUFFER,offset,sizeof(points),points);          offset += sizeof(points);
        glBufferSubData(GL_ARRAY_BUFFER,offset,sizeof(colors),colors);          offset += sizeof(colors);
        
        offset = 0;
        glGenVertexArrays(1,&RGBcubeVAO);
        glBindVertexArray(RGBcubeVAO);
        glEnableVertexAttribArray(CvPosition);
        glEnableVertexAttribArray(CvColor);
        glVertexAttribPointer(CvPosition,4,GL_FLOAT,GL_FALSE,0,VOID(offset));   offset += sizeof(points);
        glVertexAttribPointer(CvColor   ,4,GL_FLOAT,GL_FALSE,0,VOID(offset));   offset += sizeof(colors);
    }
    else {
        glBindVertexArray(0);
        glDeleteVertexArrays(1,&RGBcubeVAO);
        glDeleteBuffers(1,&RGBcubeVBO);
    }
    uninitialized = not uninitialized;
}


void initBGRcube ( ) {
    
    static GLuint BGRcubeVBO = 0;
    
    static bool uninitialized = true;
    if (uninitialized) {
        
        const vec4 vCube[8] = {
            vec4( 0,0,0,1), // right lower back
            vec4( 0,0,1,1), // right lower front
            vec4( 0,1,0,1), // right upper back
            vec4( 0,1,1,1), // right upper front
            vec4(-1,0,0,1), // left  lower back
            vec4(-1,0,1,1), // left  lower front
            vec4(-1,1,0,1), // left  upper back
            vec4(-1,1,1,1)  // left  upper front
        };
        const unsigned int iCube[6][4] = {
            {4,6,2,0}, // back
            {5,4,0,1}, // bottom
            {3,1,0,2}, // left
            {2,6,7,3}, // top
            {5,7,6,4}, // right
            {1,3,7,5}  // front
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
        
        vec4 points[36], colors[36];
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
        
        currentShader(Cshader);
        
        glGenBuffers(1,&BGRcubeVBO);
        glBindBuffer(GL_ARRAY_BUFFER,BGRcubeVBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(points)+sizeof(colors),NULL,GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER,0             ,sizeof(points),points);
        glBufferSubData(GL_ARRAY_BUFFER,sizeof(points),sizeof(colors),colors);
        checkGL(__FILE__,__LINE__);
        
        glGenVertexArrays(1,&BGRcubeVAO);
        glBindVertexArray(BGRcubeVAO);
        glEnableVertexAttribArray(CvPosition);
        glEnableVertexAttribArray(CvColor);
        glVertexAttribPointer(CvPosition,4,GL_FLOAT,GL_FALSE,0,VOID(0));
        glVertexAttribPointer(CvColor   ,4,GL_FLOAT,GL_FALSE,0,VOID(sizeof(points)));
        checkGL(__FILE__,__LINE__);
    }
    else {
        glBindVertexArray(0);
        glDeleteVertexArrays(1,&BGRcubeVAO);
        glDeleteBuffers(1,&BGRcubeVBO);
        checkGL(__FILE__,__LINE__);
    }
    uninitialized = not uninitialized;
}


void initColorcube ( ) {
    static bool uninitialized = true;
    if (uninitialized) {
        initRGBcube();
        initBGRcube();
    }
    else {
        initBGRcube();
        initRGBcube();
    }
    uninitialized = not uninitialized;
}


void drawRGBcube ( mat4 model = mat4() ) {
    currentShader(Cshader);
    glBindVertexArray(RGBcubeVAO);
    glUniformMatrix4fv(SSmodel,1,GL_FALSE,value_ptr(model));
    glDrawArrays(GL_TRIANGLES,0,36);
}


void drawBGRcube ( mat4 model = mat4() ) {
    currentShader(Cshader);
    glBindVertexArray(BGRcubeVAO);
    glUniformMatrix4fv(SSmodel,1,GL_FALSE,value_ptr(model));
    glDrawArrays(GL_TRIANGLES,0,36);
}


void drawColorcube ( mat4 model = mat4() ) {
    mat4 b, a = mat4();
    b = model*a;
    drawRGBcube(b);
    drawBGRcube(b);
    a = rotate(a,180.0f,vec3(0,1,0));
    b = model*a;
    drawRGBcube(b);
    drawBGRcube(b);
    a = rotate(a,180.0f,vec3(1,0,0));
    b = model*a;
    drawRGBcube(b);
    drawBGRcube(b);
    a = rotate(a,180.0f,vec3(0,1,0));
    b = model*a;
    drawRGBcube(b);
    drawBGRcube(b);
}
