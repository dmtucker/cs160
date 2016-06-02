#define TERRAIN_COLOR CSS_SILVER

const unsigned int terrainTriangles = 512*512*2; //XXX

GLuint terrainVAO = 0;

vec4 Tpoints [terrainTriangles*3],
     Tcolors [terrainTriangles*3],
     Tnormals[terrainTriangles*3];


void initTerrain ( ) {
    
    GLuint terrainVBO = 0;
    
    static bool uninitialized = true;
    if (uninitialized) {
        
CHECKPOINT("CHECKPOINT"); checkGL(__FILE__,__LINE__); //XXX
        GLuint p = 0;
        for (int i=1; i <= terrain.height ;++i) {
            for (int j=1; j <= terrain.width ;++j) {
                
                vec4 a, b, c, normal;
                
                a = Tpoints[p  ] = vec4(j-1,RED(PIXEL(terrain,i-1,j-1)),i-1,1); // upper left
                b = Tpoints[p+1] = vec4(j-1,RED(PIXEL(terrain,i  ,j-1)),i  ,1); // lower left
                c = Tpoints[p+2] = vec4(j  ,RED(PIXEL(terrain,i  ,j  )),i  ,1); // lower right
                
                Tcolors[p+2] = Tcolors[p+1] = Tcolors[p] = palette[TERRAIN_COLOR];
                
                normal = vec4(normalize(cross(vec3(b-a),vec3(c-b))),0);
                Tnormals [p+2] = Tnormals[p+1] = Tnormals[p] = normal;
                
                p += 3;
                
                a = Tpoints[p  ] = vec4(j-1,RED(PIXEL(terrain,i-1,j-1)),i-1,1); // upper left
                b = Tpoints[p+1] = vec4(j  ,RED(PIXEL(terrain,i  ,j  )),i  ,1); // lower right
                c = Tpoints[p+2] = vec4(j  ,RED(PIXEL(terrain,i-1,j  )),i-1,1); // upper right
                
                Tcolors[p+2] = Tcolors[p+1] = Tcolors[p] = palette[TERRAIN_COLOR];
                
                normal = vec4(normalize(cross(vec3(b-a),vec3(c-b))),0);
                Tnormals[p+2] = Tnormals[p+1] = Tnormals[p] = normal;
                
                p += 3;
            }
        }
CHECKPOINT("CHECKPOINT"); checkGL(__FILE__,__LINE__); //XXX
        
        currentShader(dynamicShader);
        
        glGenBuffers(1,&terrainVBO);
        glBindBuffer(GL_ARRAY_BUFFER,terrainVBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(Tpoints)+sizeof(Tcolors)+sizeof(Tnormals),
            NULL,
            GL_STATIC_DRAW
        );
        size_t offset = 0;
        glBufferSubData(GL_ARRAY_BUFFER,offset,sizeof(Tpoints) ,Tpoints ); offset += sizeof(Tpoints);
        glBufferSubData(GL_ARRAY_BUFFER,offset,sizeof(Tcolors) ,Tcolors ); offset += sizeof(Tcolors);
        glBufferSubData(GL_ARRAY_BUFFER,offset,sizeof(Tnormals),Tnormals); offset += sizeof(Tnormals);
        checkGL(__FILE__,__LINE__);
        
        offset = 0;
        glGenVertexArrays(1,&terrainVAO);
        glBindVertexArray(terrainVAO);
        glEnableVertexAttribArray(DSVposition);
        glEnableVertexAttribArray(DSVcolor);
        glEnableVertexAttribArray(DSVnormal);
        glVertexAttribPointer(DSVposition,4,GL_FLOAT,GL_FALSE,0,VOID(offset)); offset += sizeof(Tpoints);
        glVertexAttribPointer(DSVcolor   ,4,GL_FLOAT,GL_FALSE,0,VOID(offset)); offset += sizeof(Tcolors);
        glVertexAttribPointer(DSVnormal  ,4,GL_FLOAT,GL_FALSE,0,VOID(offset)); offset += sizeof(Tnormals);
    }
    else {
        glBindVertexArray(0);
        glDeleteVertexArrays(1,&terrainVAO);
        glDeleteBuffers(1,&terrainVBO);
        checkGL(__FILE__,__LINE__);
    }
    uninitialized = not uninitialized;
}


void drawTerrain ( mat4 model = mat4() ) {
    
    currentShader(dynamicShader);
    glBindVertexArray(terrainVAO);
    glUniformMatrix4fv(DSmodel,1,GL_FALSE,value_ptr(model));
    
    const GLfloat material_shininess = 100;
    const vec4    material_ambient   = vec4(1.0,0.0,1.0,1.0),
                  material_diffuse   = vec4(1.0,0.8,0.0,1.0),
                  material_specular  = vec4(1.0,0.8,0.0,1.0);
    
// bright
//    const GLfloat material_shininess = 100;
//    const vec4    material_ambient   = vec4(1.0,1.0,1.0,1.0),
//                  material_diffuse   = vec4(1.0,1.0,1.0,1.0),
//                  material_specular  = vec4(1.0,1.0,1.0,1.0);
    
    glUniform1f (DSMshininess,  material_shininess);
    glUniform4fv(DSMambient  ,1,value_ptr(material_ambient));
    glUniform4fv(DSMdiffuse  ,1,value_ptr(material_diffuse));
    glUniform4fv(DSMspecular ,1,value_ptr(material_specular));
    
    glDrawArrays(GL_TRIANGLES,0,terrainTriangles*3);
    checkGL(__FILE__,__LINE__);
}
