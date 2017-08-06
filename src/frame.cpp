#include "drawings/cartesian.cpp"
#include "drawings/sphere.cpp"
#include "drawings/terrain.cpp"
#include "drawings/skeleton.cpp"
#include "drawings/colorcube.cpp"
#include "drawings/fractal.cpp"


void initFrame ( ) {
    static bool uninitialized = true;
    if (uninitialized) {

        initCartesian();
        initSphere();
        initTerrain();
        initSkeleton();
        initColorcube();
        initSierpinski();

        pEye   = glm::vec4(terrainCanvas.width/2,200,terrainCanvas.height/2,1);
        pFocus = AXIS_ORIGIN;
    }
    else {
        initSierpinski();
        initColorcube();
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
        "%dx%d (%d,%d) | %d FPS | %d%% | %d° FOV | %s %s | %d",
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


void drawFrame ( ) {

    GLfloat s = 10;

                  drawCartesian();
    if (lighting) drawSphere   (glm::translate(glm::mat4(),glm::vec3(pSelection[0])));
    if (terrain) drawTerrain();
    if (colorcube) drawColorcube(glm::scale(glm::mat4(), glm::vec3(s, s, s)));
    if (sierpinski) drawSierpinski(glm::scale(glm::mat4(), glm::vec3(s, s, s)));
    if (skeleton) drawSkeleton(glm::translate(glm::mat4(), glm::vec3(pSelection[1])));
}
