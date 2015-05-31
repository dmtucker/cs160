#include "drawings/cartesian.cpp"
#include "drawings/sphere.cpp"
#include "drawings/terrain.cpp"
#include "drawings/skeleton.cpp"

//#include "drawings/colorcube.cpp" //XXX
//#include "drawings/fractal.cpp" //XXX


void initFrame ( ) {
    static bool uninitialized = true;
    if (uninitialized) {
        
        initCartesian();
        initSphere();
        initTerrain();
        initSkeleton();
        
//        initColorcube(); //XXX
//        initSierpinski(); //XXX
        
        pEye   = glm::vec4(terrain.width/2,200,terrain.height/2,1);
        pFocus = AXIS_ORIGIN;
    }
    else {
        
//        initSierpinski(); //XXX
//        initColorcube(); //XXX
        
        initSkeleton();
        initTerrain();
        initSphere();
        initCartesian();
    }
    uninitialized = not uninitialized;
}


void drawTitle ( ) {
    sprintf(
        &title[0],
        "%dx%d (%d,%d) | %d FPS | %d%% | %d FOV | %s %s | %d",
        (int) windowW,   (int) windowH,
        (int) pCursor.x, (int) pCursor.y,
        (int) round(FPS),
        (int) round(zoom*100.0),
        (int) round(lens),
        (cull) ? "culled" : "unculled",
        (wire) ?   "wire" : "solid",
        selection
    );
    glfwSetWindowTitle(defaultWindow,&title[0]);
}

bool
    skeleton1 = true,
    skeleton2 = false,
    skeleton3 = false,
    skeleton4 = false,
    skeleton5 = false,
    skeleton6 = false,
    skeleton7 = false,
    skeleton8 = false,
    skeleton9 = false;

void drawFrame ( ) {
    
                   drawCartesian();
                   drawTerrain  ();
    if (lighting)  drawSphere   (glm::translate(glm::mat4(),glm::vec3(pSelection[0])));
    if (skeleton1) drawSkeleton (1,glm::translate(glm::mat4(),glm::vec3(pSelection[1])));
    if (skeleton2) drawSkeleton (2,glm::translate(glm::mat4(),glm::vec3(pSelection[2])));
    if (skeleton3) drawSkeleton (3,glm::translate(glm::mat4(),glm::vec3(pSelection[3])));
    if (skeleton4) drawSkeleton (4,glm::translate(glm::mat4(),glm::vec3(pSelection[4])));
    if (skeleton5) drawSkeleton (5,glm::translate(glm::mat4(),glm::vec3(pSelection[5])));
    if (skeleton6) drawSkeleton (6,glm::translate(glm::mat4(),glm::vec3(pSelection[6])));
    if (skeleton7) drawSkeleton (7,glm::translate(glm::mat4(),glm::vec3(pSelection[7])));
    if (skeleton8) drawSkeleton (8,glm::translate(glm::mat4(),glm::vec3(pSelection[8])));
    if (skeleton9) drawSkeleton (9,glm::translate(glm::mat4(),glm::vec3(pSelection[9])));
    
//    GLfloat s = 10;
//     drawColorcube(scale(mat4(),vec3(s,s,s))); //XXX
//    drawSierpinski(scale(mat4(),vec3(s,s,s))); //XXX
}
