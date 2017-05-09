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
        (int) round(deg(lens)),
        (cull) ? "culled" : "unculled",
        (wire) ?   "wire" : "solid",
        selection
    );
    glfwSetWindowTitle(defaultWindow,&title[0]);
}

bool skeleton = false;

void drawFrame ( ) {

                  drawCartesian();
                  drawTerrain  ();
    if (lighting) drawSphere   (glm::translate(glm::mat4(),glm::vec3(pSelection[0])));
    if (skeleton) drawSkeleton (glm::translate(glm::mat4(),glm::vec3(pSelection[1])));

//    GLfloat s = 10;
//     drawColorcube(scale(mat4(),vec3(s,s,s))); //XXX
//    drawSierpinski(scale(mat4(),vec3(s,s,s))); //XXX
}
