const unsigned int iterations = 7;
const unsigned int sphereTriangles = pow(4,iterations+1);
vec4 Spoints[sphereTriangles*3], Scolors[sphereTriangles*3];
GLuint sphereVAO = 0, sphereVBO = 0;
mat4 sphereModel = mat4();

void recursiveTriangle ( const vec4 a , const vec4 b , const vec4 c , int level ) {
    if (level > 0) {
        
        vec4 m[3];
        m[0] = (a+b)/2.0;
        m[1] = (a+c)/2.0;
        m[2] = (b+c)/2.0;
        
        for (int i=0 ; i < 3 ;++i) {
            m[i] = normalize(vec4(m[i].x,m[i].y,m[i].z,0.0));
            m[i] = vec4(m[i].x,m[i].y,m[i].z,1.0);
        }
        
        recursiveTriangle(a   ,m[0],m[1],level-1);
        recursiveTriangle(m[0],b   ,m[2],level-1);
        recursiveTriangle(m[1],m[2],c   ,level-1);
        recursiveTriangle(m[0],m[2],m[1],level-1);
    }
    else {
        static unsigned int i = 0;
        Spoints[i] = a;
        brushColor = BASIC_RED;
        Scolors[i] = palette[brushColor];
        ++i;
        Spoints[i] = b;
        brushColor = BASIC_GREEN;
        Scolors[i] = palette[brushColor];
        ++i;
        Spoints[i] = c;
        brushColor = BASIC_BLUE;
        Scolors[i] = palette[brushColor];
        ++i;
    }
}


void recursiveSphere ( const vec4 a , const vec4 b , const vec4 c , const vec4 d , int level ) {
//    recursiveTriangle(a,b,c,level);
//    recursiveTriangle(d,c,b,level);
//    recursiveTriangle(a,d,b,level);
//    recursiveTriangle(a,c,d,level);
    recursiveTriangle(a,c,b,level);
    recursiveTriangle(a,d,c,level);
    recursiveTriangle(a,b,d,level);
    recursiveTriangle(b,c,d,level);
}


void initSphere ( ) {
    static bool uninitialized = true;
    if (uninitialized) {
        
//        recursiveSphere(
//            vec4(0             ,0                  ,1       ,1),
//            vec4(0             ,(2.0*sqrt(2.0))/3.0,-1.0/3.0,1),
//            vec4(-sqrt(6.0)/3.0,    -sqrt(2.0) /3.0,-1.0/3.0,1),
//            vec4( sqrt(6.0)/3.0,    -sqrt(2.0) /3.0,-1.0/3.0,1),
//            iterations
//        );
        
        recursiveSphere(
            vec4(0,1,0,1),
            vec4(1,0,0,1),
            vec4(cos((4*PI)/3),0,-sin((4*PI)/3),1),
            vec4(cos((2*PI)/3),0,-sin((2*PI)/3),1),
            iterations
        );
        
        glGenBuffers(1,&sphereVBO);
        glBindBuffer(GL_ARRAY_BUFFER,sphereVBO);
        glBufferData(   GL_ARRAY_BUFFER,sizeof(Spoints)+sizeof(Scolors),NULL,GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER,0              ,sizeof(Spoints),Spoints);
        glBufferSubData(GL_ARRAY_BUFFER,sizeof(Spoints),sizeof(Scolors),Scolors);
        checkGL(__FILE__,__LINE__);
        
        glGenVertexArrays(1,&sphereVAO);
        glBindVertexArray(sphereVAO);
        checkGL(__FILE__,__LINE__);
        glEnableVertexAttribArray(shaderPosition);
        glVertexAttribPointer(shaderPosition,4,GL_FLOAT,GL_FALSE,0,VOID(0));
        glEnableVertexAttribArray(shaderColor);
        glVertexAttribPointer(shaderColor,4,GL_FLOAT,GL_FALSE,0,VOID(sizeof(Spoints)));
        checkGL(__FILE__,__LINE__);
        
        uninitialized = false;
    }
    else {
        
        glBindVertexArray(0);
        glDeleteVertexArrays(1,&sphereVAO);
        glDeleteBuffers(1,&sphereVBO);
        checkGL(__FILE__,__LINE__);
        
        uninitialized = true;
    }
}


void drawSphere ( ) {
    glBindVertexArray(sphereVAO);
    glUniformMatrix4fv(shaderModel,1,GL_FALSE,value_ptr(sphereModel));
    glDrawArrays(GL_TRIANGLES,0,sphereTriangles*3);
    checkGL(__FILE__,__LINE__);
}
