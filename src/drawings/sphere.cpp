#define SPHERE_COLOR  CSS_WHITE
#define SPHERE_LIGHT  false
#define SPHERE_SOURCE true

const GLuint sphereIterations = 4;
const GLuint sphereTriangles  = 2*pow(4,sphereIterations+1);
GPUbuffer    sphereBuffer;


void recursiveIconosphereQuadrant ( glm::vec4 a ,
                                    glm::vec4 b ,
                                    glm::vec4 c ,
                                    GLint level ,
                                    glm::vec4 * points ,
                                    glm::vec4 * colors ,
                                    glm::vec4 * normals ) {
    if (level > 0) {

        glm::vec4 m[3];
        m[0] = (a+b)/glm::vec4(2.0);
        m[1] = (a+c)/glm::vec4(2.0);
        m[2] = (b+c)/glm::vec4(2.0);

        for (GLuint i=0 ; i < 3 ;++i) {
            if (SPHERE_SOURCE) {
                m[i]   = glm::normalize(m[i]);
                m[i].w = 1.0;
            }
            else {
                m[i] = glm::normalize(glm::vec4(m[i].x,m[i].y,m[i].z,0.0));
                m[i] =                glm::vec4(m[i].x,m[i].y,m[i].z,1.0);
            }
        }

        recursiveIconosphereQuadrant(a   ,m[0],m[1],level-1,points,colors,normals);
        recursiveIconosphereQuadrant(m[0],b   ,m[2],level-1,points,colors,normals);
        recursiveIconosphereQuadrant(m[1],m[2],c   ,level-1,points,colors,normals);
        recursiveIconosphereQuadrant(m[0],m[2],m[1],level-1,points,colors,normals);
    }
    else {

        static GLuint i = 0;
        points[i] = normals[i] = a; colors[i] = palette[SPHERE_COLOR]; ++i;
        points[i] = normals[i] = b; colors[i] = palette[SPHERE_COLOR]; ++i;
        points[i] = normals[i] = c; colors[i] = palette[SPHERE_COLOR]; ++i;
    }
}


void initSphere ( bool UV = false ) {
    static bool uninitialized = true;
    if (uninitialized) {

        glm::vec4 * const points  = new glm::vec4[sphereTriangles*3];
        glm::vec4 * const colors  = new glm::vec4[sphereTriangles*3];
        glm::vec4 * const normals = new glm::vec4[sphereTriangles*3];

        if (SPHERE_SOURCE) pSelection[0] = glm::vec4(0,FAR,0,1);

        if (UV) { //XXX UV sphere
//            GLuint I = 0;
//            points[I] = vec4( 0, 1, 0, 1);
//            colors[I] = SPHERE_COLOR;
//            GLfloat delta = (2*PI)/sphereIterations;
//            for (int i = 0; i < sphereIterations ;++i) {
//
//            }
        }
        else {
            const glm::vec4 a = glm::vec4( 0, 1, 0, 1);
            const glm::vec4 b = glm::vec4( 0, 0, 1, 1);
            const glm::vec4 c = glm::vec4( 1, 0, 0, 1);
            const glm::vec4 d = glm::vec4( 0, 0,-1, 1);
            const glm::vec4 e = glm::vec4(-1, 0, 0, 1);
            const glm::vec4 f = glm::vec4( 0,-1, 0, 1);
            recursiveIconosphereQuadrant(a,b,c,sphereIterations,points,colors,normals);
            recursiveIconosphereQuadrant(a,c,d,sphereIterations,points,colors,normals);
            recursiveIconosphereQuadrant(a,d,e,sphereIterations,points,colors,normals);
            recursiveIconosphereQuadrant(a,e,b,sphereIterations,points,colors,normals);
            recursiveIconosphereQuadrant(f,c,b,sphereIterations,points,colors,normals);
            recursiveIconosphereQuadrant(f,d,c,sphereIterations,points,colors,normals);
            recursiveIconosphereQuadrant(f,e,d,sphereIterations,points,colors,normals);
            recursiveIconosphereQuadrant(f,b,e,sphereIterations,points,colors,normals);
        }

        if (SPHERE_LIGHT) CLobject(
            &sphereBuffer,
             sphereTriangles*3,
            points,
            colors,
            normals
        );
        else Cobject(
            &sphereBuffer,
             sphereTriangles*3,
            points,
            colors
        );

        delete [] normals;
        delete [] colors;
        delete [] points;
    }
    else {
        if (SPHERE_LIGHT) cleanCLobject(&sphereBuffer);
        else               cleanCobject(&sphereBuffer);
    }
    uninitialized = not uninitialized;
}


void drawSphere ( glm::mat4 model = glm::mat4() ) {

    if (SPHERE_LIGHT) {

        currentShader(CLshader);
        glBindVertexArray(sphereBuffer.VAO);

        glUniformMatrix4fv(CLmodel,1,GL_FALSE,glm::value_ptr(model));

        glUniform1f (CLmShininess,  bright_shininess);
        glUniform4fv(CLmAmbient  ,1,glm::value_ptr(bright_ambient));
        glUniform4fv(CLmDiffuse  ,1,glm::value_ptr(bright_diffuse));
        glUniform4fv(CLmSpecular ,1,glm::value_ptr(bright_specular));
    }
    else {

        currentShader(Cshader);
        glBindVertexArray(sphereBuffer.VAO);

        glUniformMatrix4fv(Cmodel,1,GL_FALSE,glm::value_ptr(model));
    }

    glDrawArrays(GL_TRIANGLES,0,sphereTriangles*3);
}
