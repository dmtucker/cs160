const unsigned int iterations = 4;
const unsigned int gasketTriangles = pow(4,iterations+1);
vec4 Gpoints[gasketTriangles*3], Gcolors[gasketTriangles*3];
GLuint gasketVAO = 0, gasketVBO = 0;
mat4 gasketModel = mat4();


void initTriangle ( const vec4 a , const vec4 b , const vec4 c ) {
    static unsigned int i = 0;
    Gpoints[i] = a;
    Gcolors[i] = palette[brushColor];
    ++i;
    Gpoints[i] = b;
    Gcolors[i] = palette[brushColor];
    ++i;
    Gpoints[i] = c;
    Gcolors[i] = palette[brushColor];
    ++i;
}


void init3DGasket ( const vec4 a , const vec4 b , const vec4 c , const vec4 d , int level ) {
    if (level > 0) {
        vec4 m[6];
        m[0] = (a+b)/2.0;
        m[1] = (a+c)/2.0;
        m[2] = (b+c)/2.0;
        m[3] = (a+d)/2.0;
        m[4] = (b+d)/2.0;
        m[5] = (c+d)/2.0;
        init3DGasket(a   ,m[0],m[1],m[3],level-1);
        init3DGasket(m[0],b   ,m[2],m[4],level-1);
        init3DGasket(m[1],m[2],c   ,m[5],level-1);
        init3DGasket(m[3],m[4],m[5],d   ,level-1);
    }
    else {
        brushColor = BASIC_RED;
        initTriangle(a,c,b);
        brushColor = BASIC_GREEN;
        initTriangle(a,d,c);
        brushColor = BASIC_BLUE;
        initTriangle(a,b,d);
        brushColor = BASIC_WHITE;
        initTriangle(b,c,d);
    }
}


void initSierpinski ( ) {
    static bool uninitialized = true;
    if (uninitialized) {
        
        init3DGasket(
            vec4(0,1,0,1),                          // middle upper  middle
            vec4(1,0,0,1),                          // middle middle back
            vec4(cos((4*PI)/3),0,-sin((4*PI)/3),1), // left   middle back
            vec4(cos((2*PI)/3),0,-sin((2*PI)/3),1), // right  middle back
            iterations
        );
        
        glGenBuffers(1,&gasketVBO);
        glBindBuffer(GL_ARRAY_BUFFER,gasketVBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(Gpoints)+sizeof(Gcolors),NULL,GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER,0             ,sizeof(Gpoints),Gpoints);
        glBufferSubData(GL_ARRAY_BUFFER,sizeof(Gpoints),sizeof(Gcolors),Gcolors);
        checkGL(__FILE__,__LINE__);
        
        glGenVertexArrays(1,&gasketVAO);
        glBindVertexArray(gasketVAO);
        checkGL(__FILE__,__LINE__);
        glEnableVertexAttribArray(shaderPosition);
        glVertexAttribPointer(shaderPosition,4,GL_FLOAT,GL_FALSE,0,VOID(0));
        glEnableVertexAttribArray(shaderColor);
        glVertexAttribPointer(shaderColor,4,GL_FLOAT,GL_FALSE,0,VOID(sizeof(Gpoints)));
        checkGL(__FILE__,__LINE__);
        
        uninitialized = false;
    }
    else {
        
        glBindVertexArray(0);
        glDeleteVertexArrays(1,&gasketVAO);
        glDeleteBuffers(1,&gasketVBO);
        checkGL(__FILE__,__LINE__);
        
        uninitialized = true;
    }
}


void drawSierpinski ( ) {
    glBindVertexArray(gasketVAO);
    glUniformMatrix4fv(shaderModel,1,GL_FALSE,value_ptr(gasketModel));
    glDrawArrays(GL_TRIANGLES,0,gasketTriangles*3);
    checkGL(__FILE__,__LINE__);
}
