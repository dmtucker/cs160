GLuint snowmanVAO = 0, snowmanVBO = 0;
mat4 snowmanModel = mat4();
vec4 snowmanColors[36];
bool wavingSnowman = false;
bool raiseArm   = false;
bool raiseElbow = false;
bool lowerArm   = false;
bool lowerElbow = false;


void initSnowman ( ) {
    
    glGenBuffers(1,&snowmanVBO);
    glBindBuffer(GL_ARRAY_BUFFER,snowmanVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(cube)+sizeof(snowmanColors),NULL,GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(cube),cube);
    checkGL(__FILE__,__LINE__);
    
    glGenVertexArrays(1,&snowmanVAO);
    glBindVertexArray(snowmanVAO);
    checkGL(__FILE__,__LINE__);
    glEnableVertexAttribArray(shaderPosition);
    glVertexAttribPointer(shaderPosition,4,GL_FLOAT,GL_FALSE,0,VOID(0));
    glEnableVertexAttribArray(shaderColor);
    glVertexAttribPointer(shaderColor,4,GL_FLOAT,GL_FALSE,0,VOID(sizeof(cube)));
    checkGL(__FILE__,__LINE__);
}


void drawSnowman ( ) {
    
    glBindVertexArray(snowmanVAO);
    
    for (int i=0; i < 36 ;++i) snowmanColors[i] = palette[BASIC_WHITE];
    glBufferSubData(GL_ARRAY_BUFFER,sizeof(cube),sizeof(snowmanColors),snowmanColors);
    
    // body
    snowmanModel = mat4();
//    snowmanModel = rotate(snowmanModel,(GLfloat) (((int) (glfwGetTime()*10))%360),vec3(AXIS_Y));
//    snowmanModel = translate(snowmanModel,vec3(1,0,1));
    glUniformMatrix4fv(shaderModel,1,GL_FALSE,value_ptr(snowmanModel));
    glDrawArrays(GL_TRIANGLES,0,36);
    
    // torso
    GLfloat s = 0.67;
    vec3    t = vec3(s,s,s);
    mat4 torsoModel = translate(snowmanModel,vec3(0,t.z+1,0));
         torsoModel = scale(torsoModel,t);
    glUniformMatrix4fv(shaderModel,1,GL_FALSE,value_ptr(torsoModel));
    glDrawArrays(GL_TRIANGLES,0,36);
    
    for (int i=0; i < 36 ;++i) snowmanColors[i] = palette[BASIC_BLACK];
    glBufferSubData(GL_ARRAY_BUFFER,sizeof(cube),sizeof(snowmanColors),snowmanColors);
    
    // middle button
    s = 0.1;
    t = vec3(s,s,0.5*s);
    mat4 buttonModel = translate(torsoModel,vec3(0,0,t.z+1));
         buttonModel = scale(buttonModel,t);
    glUniformMatrix4fv(shaderModel,1,GL_FALSE,value_ptr(buttonModel));
    glDrawArrays(GL_TRIANGLES,0,36);
    
    // upper button
    buttonModel = translate(buttonModel,vec3(0,5,0));
    glUniformMatrix4fv(shaderModel,1,GL_FALSE,value_ptr(buttonModel));
    glDrawArrays(GL_TRIANGLES,0,36);
    
    // lower button
    buttonModel = translate(buttonModel,vec3(0,-10,0));
    glUniformMatrix4fv(shaderModel,1,GL_FALSE,value_ptr(buttonModel));
    glDrawArrays(GL_TRIANGLES,0,36);
    
    for (int i=0; i < 36 ;++i) snowmanColors[i] = palette[CSS_OLIVE];
    glBufferSubData(GL_ARRAY_BUFFER,sizeof(cube),sizeof(snowmanColors),snowmanColors);
    
    // right arm
    static bool upward = true;
    static int motion = 20;
    if (wavingSnowman) {
        if (upward) {
            if (motion > -20) --motion;
            else upward = false;
        }
        else {
            if (motion <  20) ++motion;
            else upward = true;
        }
    }
    
    s = 0.2;
    t = vec3(3*s,s,s);
    mat4 armModel = torsoModel;
         armModel = rotate(armModel,(GLfloat) motion,vec3(AXIS_Z));
         armModel = rotate(armModel,20.0f,vec3(AXIS_Y));
         armModel = translate(armModel,vec3(-(t.x+0.75),0.5,-0.25));
         armModel = scale(armModel,t);
    glUniformMatrix4fv(shaderModel,1,GL_FALSE,value_ptr(armModel));
    glDrawArrays(GL_TRIANGLES,0,36);
    
    // right forearm
    static bool upwardForearm = true;
    static int motionForearm = 20;
    if (wavingSnowman) {
        if (upwardForearm) {
            if (motionForearm > -60) motionForearm -= 2;
            else upwardForearm = false;
        }
        else {
            if (motionForearm <  20) motionForearm += 2;
            else if (upward) upwardForearm = true;
        }
    }
    
    s = 0.5;
    t = vec3(3*s,s,s);
    mat4 forearmModel = armModel;
         forearmModel = rotate(forearmModel,(GLfloat) motionForearm,vec3(AXIS_Z));
         forearmModel = rotate(forearmModel,20.0f,vec3(AXIS_Y));
         forearmModel = translate(forearmModel,vec3(-(t.x+0.85),0.5,-0.25));
         forearmModel = scale(forearmModel,t);
    glUniformMatrix4fv(shaderModel,1,GL_FALSE,value_ptr(forearmModel));
    glDrawArrays(GL_TRIANGLES,0,36);
    
    // right hand
    s = 0.3;
    t = vec3(3*s,s,s);
    mat4 handModel = forearmModel;
         handModel = rotate(handModel,-20.0f,vec3(AXIS_Z));
         handModel = rotate(handModel,-20.0f,vec3(AXIS_Y));
         handModel = translate(handModel,vec3(-(t.x+0.75),0.5,-0.25));
         handModel = scale(handModel,t);
    glUniformMatrix4fv(shaderModel,1,GL_FALSE,value_ptr(handModel));
    glDrawArrays(GL_TRIANGLES,0,36);
    
    // right thumb
    s = 0.2;
    t = vec3(3*s,s,s);
    mat4 thumbModel = forearmModel;
         thumbModel = rotate(thumbModel,20.0f,vec3(AXIS_Z));
         thumbModel = rotate(thumbModel,20.0f,vec3(AXIS_Y));
         thumbModel = translate(thumbModel,vec3(-(t.x+0.75),0.5,-0.25));
         thumbModel = scale(thumbModel,t);
    glUniformMatrix4fv(shaderModel,1,GL_FALSE,value_ptr(thumbModel));
    glDrawArrays(GL_TRIANGLES,0,36);
    
    // left arm
    static int motionLeftarm = -20;
    if (raiseArm xor lowerArm) {
        if (raiseArm) {
            ++motionLeftarm;
            if (motionLeftarm > 20) motionLeftarm = 20;
        }
        else {
            --motionLeftarm;
            if (motionLeftarm < -20) motionLeftarm = -20;
        }
    }
    
    s = 0.2;
    t = vec3(3*s,s,s);
    armModel = torsoModel;
    armModel = rotate(armModel,(GLfloat) motionLeftarm,vec3(AXIS_Z));
    armModel = rotate(armModel,-20.0f,vec3(AXIS_Y));
    armModel = translate(armModel,vec3(t.x+0.75,0.5,-0.25));
    armModel = scale(armModel,t);
    glUniformMatrix4fv(shaderModel,1,GL_FALSE,value_ptr(armModel));
    glDrawArrays(GL_TRIANGLES,0,36);
    
    // left forearm
    static int motionLeftForearm = -20;
    if (raiseElbow xor lowerElbow) {
        if (raiseElbow) {
            ++motionLeftForearm;
            if (motionLeftForearm > 60) motionLeftForearm = 60;
        }
        else {
            --motionLeftForearm;
            if (motionLeftForearm < -60) motionLeftForearm = -60;
        }
    }
    
    s = 0.5;
    t = vec3(3*s,s,s);
    forearmModel = armModel;
    forearmModel = rotate(forearmModel,(GLfloat) motionLeftForearm,vec3(AXIS_Z));
    forearmModel = rotate(forearmModel,-20.0f,vec3(AXIS_Y));
    forearmModel = translate(forearmModel,vec3(t.x+0.75,0.5,-0.25));
    forearmModel = scale(forearmModel,t);
    glUniformMatrix4fv(shaderModel,1,GL_FALSE,value_ptr(forearmModel));
    glDrawArrays(GL_TRIANGLES,0,36);
    
    // left hand
    s = 0.3;
    t = vec3(3*s,s,s);
    handModel = forearmModel;
    handModel = rotate(handModel,20.0f,vec3(AXIS_Z));
    handModel = rotate(handModel,20.0f,vec3(AXIS_Y));
    handModel = translate(handModel,vec3(t.x+0.75,0.5,-0.25));
    handModel = scale(handModel,t);
    glUniformMatrix4fv(shaderModel,1,GL_FALSE,value_ptr(handModel));
    glDrawArrays(GL_TRIANGLES,0,36);
    
    // left thumb
    s = 0.2;
    t = vec3(3*s,s,s);
    thumbModel = forearmModel;
    thumbModel = rotate(thumbModel,-20.0f,vec3(AXIS_Z));
    thumbModel = rotate(thumbModel,-20.0f,vec3(AXIS_Y));
    thumbModel = translate(thumbModel,vec3(t.x+0.75,0.5,-0.25));
    thumbModel = scale(thumbModel,t);
    glUniformMatrix4fv(shaderModel,1,GL_FALSE,value_ptr(thumbModel));
    glDrawArrays(GL_TRIANGLES,0,36);
    
    for (int i=0; i < 36 ;++i) snowmanColors[i] = palette[BASIC_WHITE];
    glBufferSubData(GL_ARRAY_BUFFER,sizeof(cube),sizeof(snowmanColors),snowmanColors);
    
    // head
    GLfloat h = 0.67;
    mat4 headModel = torsoModel;
         headModel = translate(headModel,vec3(0,h+1,0));
         headModel = scale(headModel,vec3(h,h,h));
    glUniformMatrix4fv(shaderModel,1,GL_FALSE,value_ptr(headModel));
    glDrawArrays(GL_TRIANGLES,0,36);
    
    for (int i=0; i < 36 ;++i) snowmanColors[i] = palette[BASIC_BLACK];
    glBufferSubData(GL_ARRAY_BUFFER,sizeof(cube),sizeof(snowmanColors),snowmanColors);
    
    // right eye
    s = 0.1;
    t = vec3(s,s,0.5*s);
    buttonModel = headModel;
    buttonModel = translate(buttonModel,vec3(-0.35,0.25,t.z+1));
    buttonModel = scale(buttonModel,t);
    glUniformMatrix4fv(shaderModel,1,GL_FALSE,value_ptr(buttonModel));
    glDrawArrays(GL_TRIANGLES,0,36);
    
    // left eye
    s = 0.1;
    t = vec3(s,s,0.5*s);
    buttonModel = headModel;
    buttonModel = translate(buttonModel,vec3( 0.35,0.25,t.z+1));
    buttonModel = scale(buttonModel,t);
    glUniformMatrix4fv(shaderModel,1,GL_FALSE,value_ptr(buttonModel));
    glDrawArrays(GL_TRIANGLES,0,36);
    
    for (int i=0; i < 36 ;++i) snowmanColors[i] = palette[CSS_OLIVE];
    glBufferSubData(GL_ARRAY_BUFFER,sizeof(cube),sizeof(snowmanColors),snowmanColors);
    
    // pipe
    s = 0.05;
    t = vec3(s,s,12*s);
    mat4 pipeModel = headModel;
         pipeModel = translate(pipeModel,vec3(0,-0.25,t.z+1));
         pipeModel = scale(pipeModel,t);
    glUniformMatrix4fv(shaderModel,1,GL_FALSE,value_ptr(pipeModel));
    glDrawArrays(GL_TRIANGLES,0,36);
    
    // pipe bowl
    s = 2.0;
    t = vec3(s,1.5*s,s/12);
    mat4 bowlModel = pipeModel;
         bowlModel = translate(bowlModel,vec3(0,0.5,1.7*(t.z+0.5)));
         bowlModel = scale(bowlModel,t);
    glUniformMatrix4fv(shaderModel,1,GL_FALSE,value_ptr(bowlModel));
    glDrawArrays(GL_TRIANGLES,0,36);
    
    checkGL(__FILE__,__LINE__);
}

void cleanSnowman ( ) {
    //XXX
}
