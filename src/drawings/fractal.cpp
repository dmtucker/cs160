#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078

const unsigned int gasketIterations = 4;
const unsigned int gasketTriangles  = pow(4,gasketIterations+1);

GLuint gasketVAO   = 0;

glm::vec4 Gpoints[gasketTriangles*3], Gcolors[gasketTriangles*3]; //XXX Pass these from initSierpinski to recursiveSierpinski for the bottom level of recursion.


void initGasketTriangle ( const glm::vec4 a , const glm::vec4 b , const glm::vec4 c ) { //XXX Integrate this into recursiveSierpinski.
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


void recursiveSierpinski ( const glm::vec4 a , const glm::vec4 b , const glm::vec4 c , const glm::vec4 d , int level ) {
    if (level > 0) {
        glm::vec4 m[6];
        m[0] = (a+b)/glm::vec4(2.0);
        m[1] = (a+c)/glm::vec4(2.0);
        m[2] = (b+c)/glm::vec4(2.0);
        m[3] = (a+d)/glm::vec4(2.0);
        m[4] = (b+d)/glm::vec4(2.0);
        m[5] = (c+d)/glm::vec4(2.0);
        recursiveSierpinski(a   ,m[0],m[1],m[3],level-1);
        recursiveSierpinski(m[0],b   ,m[2],m[4],level-1);
        recursiveSierpinski(m[1],m[2],c   ,m[5],level-1);
        recursiveSierpinski(m[3],m[4],m[5],d   ,level-1);
    }
    else { // initTetrahedron
        brushColor = BASIC_RED;
        initGasketTriangle(a,c,b);
        brushColor = BASIC_GREEN;
        initGasketTriangle(a,d,c);
        brushColor = BASIC_BLUE;
        initGasketTriangle(a,b,d);
        brushColor = BASIC_WHITE;
        initGasketTriangle(b,c,d);
    }
}


void initSierpinski ( ) {

    static GLuint gasketVBO = 0;

    static bool uninitialized = true;
    if (uninitialized) {

        recursiveSierpinski(
            glm::vec4(0,1,0,1),
            glm::vec4(1,0,0,1),
            glm::vec4(cos((4*PI)/3),0,-sin((4*PI)/3),1),
            glm::vec4(cos((2*PI)/3),0,-sin((2*PI)/3),1),
            gasketIterations
        );

        currentShader(Cshader);

        glGenBuffers(1,&gasketVBO);
        glBindBuffer(GL_ARRAY_BUFFER,gasketVBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(Gpoints)+sizeof(Gcolors),NULL,GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER,0              ,sizeof(Gpoints),Gpoints);
        glBufferSubData(GL_ARRAY_BUFFER,sizeof(Gpoints),sizeof(Gcolors),Gcolors);
        checkGL(__FILE__,__LINE__);

        glGenVertexArrays(1,&gasketVAO);
        glBindVertexArray(gasketVAO);
        glEnableVertexAttribArray(CvPosition);
        glEnableVertexAttribArray(CvColor);
        glVertexAttribPointer(CvPosition,4,GL_FLOAT,GL_FALSE,0,VOID(0));
        glVertexAttribPointer(CvColor,4,GL_FLOAT,GL_FALSE,0,VOID(sizeof(Gpoints)));
        checkGL(__FILE__,__LINE__);
    }
    else {

        glBindVertexArray(0);
        glDeleteVertexArrays(1,&gasketVAO);
        glDeleteBuffers(1,&gasketVBO);
        checkGL(__FILE__,__LINE__);
    }
    uninitialized = not uninitialized;
}


void drawSierpinski ( glm::mat4 model = glm::mat4() ) {
    currentShader(Cshader);
    glBindVertexArray(gasketVAO);
    glUniformMatrix4fv(Cmodel,1,GL_FALSE,value_ptr(model));
    glDrawArrays(GL_TRIANGLES,0,gasketTriangles*3);
    checkGL(__FILE__,__LINE__);
}
