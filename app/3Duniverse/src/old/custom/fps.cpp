
    
    
    
//    static vec2 theta = vec2(0,0);       const double delta = 1.0;
//    theta.x += (GLfloat) linear_map(pCursor.y,0,windowH, delta,-delta);
//    theta.y += (GLfloat) linear_map(pCursor.x,0,windowW,-delta, delta);
//    vec4 vForward = normalize(pFocus-pEye);
//    vec3 vLeft    = normalize(cross(vec3(vUp),vec3(vForward)));
//    vForward = rotate(mat4(),theta.x,vLeft)*vForward;
//    pFocus = pEye+vForward;
//    
    
/*/XXX FPS Camera
    static vec2 theta = vec2(90,90);       const double delta = 1.0;
//    theta.x += (GLfloat) linear_map(pCursor.y,0,windowH, delta,-delta);
//    theta.y += (GLfloat) linear_map(pCursor.x,0,windowW,-delta, delta);
    theta.x += (GLfloat) linear_map(proj(vCursor,vec3(AXIS_Y)).y,-windowH/2.0,windowH/2.0,-delta,delta);
    theta.y += 0;//(GLfloat) linear_map(proj(vCursor,vec3(AXIS_X)).x,-windowW/2.0,windowW/2.0,-delta,delta);
    if (theta.x <    0) theta.x  =   0;
    if (theta.x >  180) theta.x  = 180;
    if (theta.y < -360) theta.y += 360;
    if (theta.y >  360) theta.y -= 360;
    GLfloat inclination = theta.x*DEG2RAD;
    GLfloat azimuth     = theta.y*DEG2RAD;
    vec3 vLook = vec3(
         sin(azimuth)*cos(inclination),
         cos(azimuth)
        -sin(azimuth)*sin(inclination)
    );
//    pFocus = translate(mat4(),pEye)*vec4(normalize(pEye+pLook),0));
    pFocus = translate(mat4(),vec3(pEye))*vec4(vLook,0);
/**/
