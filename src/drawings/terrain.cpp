#define TERRAIN_COLOR CSS_WHITE

GLuint    terrainTriangles = 0;
GPUbuffer terrainBuffer;
GLuint    terrainTexture   = 0;


void initTerrain ( ) {
    static bool uninitialized = true;
    if (uninitialized) {

        terrainTriangles = terrain.width*terrain.height*2;

        glm::vec4 * const points   = new glm::vec4[terrainTriangles*3];
        glm::vec4 * const colors   = new glm::vec4[terrainTriangles*3];
        glm::vec4 * const normals  = new glm::vec4[terrainTriangles*3];
        glm::vec2 * const textures = new glm::vec2[terrainTriangles*3];

        GLuint p = 0;
        for (int i=1; i < terrain.height ;++i) {
            for (int j=1; j < terrain.width ;++j) {

                glm::vec4 a, b, c, normal;

                a = points[p  ] = glm::vec4(j-1,RED(PIXEL(terrain,j-1,i-1)),i-1,1);  // upper left
                b = points[p+1] = glm::vec4(j-1,RED(PIXEL(terrain,j-1,i  )),i  ,1);  // lower left
                c = points[p+2] = glm::vec4(j  ,RED(PIXEL(terrain,j  ,i  )),i  ,1);  // lower right

                colors[p+2] = colors[p+1] = colors[p] = palette[TERRAIN_COLOR];

                textures[p  ] = glm::vec2((j-1)/(GLfloat) terrain.width,(i-1)/(GLfloat) terrain.height);
                textures[p+1] = glm::vec2((j-1)/(GLfloat) terrain.width,(i  )/(GLfloat) terrain.height);
                textures[p+2] = glm::vec2((j  )/(GLfloat) terrain.width,(i  )/(GLfloat) terrain.height);

                normal = glm::vec4(glm::normalize(glm::cross(glm::vec3(b-a),glm::vec3(c-b))),0);
                normals [p+2] = normals[p+1] = normals[p] = normal;

                p += 3;

                a = points[p  ] = glm::vec4(j-1,RED(PIXEL(terrain,j-1,i-1)),i-1,1);  // upper left
                b = points[p+1] = glm::vec4(j  ,RED(PIXEL(terrain,j  ,i  )),i  ,1);  // lower right
                c = points[p+2] = glm::vec4(j  ,RED(PIXEL(terrain,j  ,i-1)),i-1,1);  // upper right

                colors[p+2] = colors[p+1] = colors[p] = palette[TERRAIN_COLOR];

                textures[p  ] = glm::vec2((j-1)/(GLfloat) terrain.width,(i-1)/(GLfloat) terrain.height);
                textures[p+1] = glm::vec2((j  )/(GLfloat) terrain.width,(i  )/(GLfloat) terrain.height);
                textures[p+2] = glm::vec2((j  )/(GLfloat) terrain.width,(i-1)/(GLfloat) terrain.height);

                normal = glm::vec4(glm::normalize(glm::cross(glm::vec3(b-a),glm::vec3(c-b))),0);
                normals[p+2] = normals[p+1] = normals[p] = normal;

                p += 3;
            }
        }

        CTLobject(
            &terrainBuffer,
             terrainTriangles*3,
            points,
            colors,
            textures,
            normals
        );

        glGenTextures(1,&terrainTexture);
        glBindTexture(GL_TEXTURE_2D,terrainTexture);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB8,
            texture.width,
            texture.height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            texture.pixels
        );

        delete [] textures;
        delete [] normals;
        delete [] colors;
        delete [] points;
    }
    else {
        glDeleteTextures(1,&terrainTexture);
        cleanCTLobject(&terrainBuffer);
    }
    uninitialized = not uninitialized;
}


void drawTerrain ( glm::mat4 model = glm::mat4() ) {

    currentShader(CTLshader);
    glBindVertexArray(terrainBuffer.VAO);
    glBindTexture(GL_TEXTURE_2D,terrainTexture);

    glUniformMatrix4fv(CTLmodel,1,GL_FALSE,glm::value_ptr(model));

    const GLfloat material_shininess = 100;
    const glm::vec4
        material_ambient   = glm::vec4(1.0,0.0,1.0,1.0),
        material_diffuse   = glm::vec4(1.0,0.8,0.0,1.0),
        material_specular  = glm::vec4(1.0,0.8,0.0,1.0);

    glUniform1f (CTLmShininess,  material_shininess);
    glUniform4fv(CTLmAmbient  ,1,glm::value_ptr(material_ambient));
    glUniform4fv(CTLmDiffuse  ,1,glm::value_ptr(material_diffuse));
    glUniform4fv(CTLmSpecular ,1,glm::value_ptr(material_specular));

    glDrawArrays(GL_TRIANGLES,0,terrainTriangles*3);
}
