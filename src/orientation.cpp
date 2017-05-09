#define AXIS_X      glm::vec4(1,0,0,0)
#define AXIS_Y      glm::vec4(0,1,0,0)
#define AXIS_Z      glm::vec4(0,0,1,0)
#define AXIS_ORIGIN glm::vec4(0,0,0,1)

#define FAR        1000.0f // world units
#define PROJECTION (lens) ? glm::perspective(lens,ASPECT_RATIO,0.1f,FAR*2) \
                          : glm::ortho(-FAR/10,FAR/10,-FAR/10,FAR/10,0.1f,FAR)

GLfloat
    lens = rad(60),
    zoom = 1;

bool lighting = true;

glm::vec4
    light_ambient  = glm::vec4(0.2,0.2,0.2,1.0),
    light_diffuse  = glm::vec4(1.0,1.0,1.0,1.0),
    light_specular = glm::vec4(1.0,1.0,1.0,1.0);

glm::vec4
    pEye   = AXIS_ORIGIN,
    pFocus = glm::vec4(0,0,-1,1),
    vUp    = AXIS_Y;

bool
    selectionMoveL[10], // left
    selectionMoveR[10], // right
    selectionMoveU[10], // up
    selectionMoveD[10], // down
    selectionMoveB[10], // back
    selectionMoveF[10]; // forth

GLuint selection = 1;
glm::vec4 pSelection[10];


bool
    moveL = false, lookL = false,
    moveR = false, lookR = false,
    moveU = false, lookU = false,
    moveD = false, lookD = false,
    moveB = false,
    moveF = false,

    orbitCW  = false,
    orbitCCW = false;


void initOrientation ( ) {
    static bool uninitialized = true;
    if (uninitialized) {
        for (int i=0; i < 10 ;++i) {
            pSelection[i] = AXIS_ORIGIN;
             selectionMoveL[i] = false;
             selectionMoveR[i] = false;
             selectionMoveU[i] = false;
             selectionMoveD[i] = false;
             selectionMoveB[i] = false;
             selectionMoveF[i] = false;
        }
    }
    uninitialized = not uninitialized;
}


void reorient ( ) {

    glm::vec4 vForward = glm::normalize(pFocus-pEye);
    glm::vec4 vAhead   = glm::normalize(glm::vec4(vForward.x,0,vForward.z,vForward.w));
    glm::vec4 vRight   = glm::normalize(glm::vec4(glm::cross(glm::vec3(vForward),glm::vec3(vUp)),0));
    pFocus = vForward+pEye;

    glm::mat4 orbit = glm::mat4();
    const GLfloat velocity = rad(3.0);
    if (orbitCW xor orbitCCW) orbit = (orbitCCW) ?
                  glm::rotate(orbit, velocity,glm::vec3(vUp)):
                  glm::rotate(orbit,-velocity,glm::vec3(vUp));
    pEye = orbit*pEye;

    glm::mat4 position = glm::mat4();
    const GLfloat agility = 3.0;
    if (moveL xor moveR) position = (moveR) ?
          glm::translate(position,glm::vec3( vRight  *agility)):
          glm::translate(position,glm::vec3(-vRight  *agility));
    if (moveU xor moveD) position = (moveU) ?
          glm::translate(position,glm::vec3( vUp     *agility)):
          glm::translate(position,glm::vec3(-vUp     *agility));
    if (moveB xor moveF) position = (moveF) ?
          glm::translate(position,glm::vec3( vForward*agility)):
          glm::translate(position,glm::vec3(-vForward*agility));
    pEye   = position*pEye;
    pFocus = position*pFocus;

    glm::mat4 attention = glm::mat4();
    const GLfloat sensitivity = rad(3);
    if (lookL xor lookR) attention = (lookL) ?
             glm::rotate(attention, sensitivity,glm::vec3(vUp)):
             glm::rotate(attention,-sensitivity,glm::vec3(vUp));
    if (lookU xor lookD) attention = (lookU) ?
             glm::rotate(attention, sensitivity,glm::vec3(vRight)):
             glm::rotate(attention,-sensitivity,glm::vec3(vRight));
    pFocus = pEye+(attention*vForward);

    glm::mat4 view = glm::lookAt(glm::vec3(pEye),glm::vec3(pFocus),glm::vec3(vUp));
              view = glm::scale(view,glm::vec3(zoom,zoom,zoom));
    updateView(view);
    updateProjection(PROJECTION);


    position = glm::mat4();
    if (selectionMoveL[selection] xor selectionMoveR[selection])
        position = (selectionMoveR[selection]) ?
            glm::translate(position,glm::vec3( vRight*agility)):
            glm::translate(position,glm::vec3(-vRight*agility));
    if (selectionMoveU[selection] xor selectionMoveD[selection])
        position = (selectionMoveU[selection]) ?
            glm::translate(position,glm::vec3( vUp   *agility)):
            glm::translate(position,glm::vec3(-vUp   *agility));
    if (selectionMoveB[selection] xor selectionMoveF[selection])
        position = (selectionMoveF[selection]) ?
            glm::translate(position,glm::vec3( vAhead*agility)):
            glm::translate(position,glm::vec3(-vAhead*agility));
    pSelection[selection] = position*pSelection[selection];


    updateLight(pSelection[0],light_ambient,light_diffuse,light_specular);
}


bool toggleLight ( bool enable = not lighting ) {

    static glm::vec4
        previous_ambient,
        previous_diffuse,
        previous_specular;

    if (enable) {
        light_ambient  = previous_ambient;
        light_diffuse  = previous_diffuse;
        light_specular = previous_specular;
    }
    else {
        previous_ambient  = light_ambient;
        previous_diffuse  = light_diffuse;
        previous_specular = light_specular;

        const GLfloat dark = 0.025;
        light_ambient  = glm::vec4(dark,dark,dark,1);
        light_diffuse  = glm::vec4(dark,dark,dark,1);
        light_specular = glm::vec4(   0,   0,   0,1);
    }
    lighting = enable;
}
