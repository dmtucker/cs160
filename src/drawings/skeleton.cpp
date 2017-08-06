bool skeleton = false;
GPUbuffer boneBuffer;

typedef struct bone_t {
    glm::mat4 model;
    void (*draw)( );
    struct bone_t * child;
    struct bone_t * sibling;
} bone_t;

bone_t
    skeletonTorso,
    skeletonHead,
    skeletonRclavicle, skeletonLclavicle,
    skeletonRhumerus,  skeletonLhumerus,
    skeletonRradius,   skeletonLradius,
    skeletonRpelvis,   skeletonLpelvis,
    skeletonRfemur,    skeletonLfemur,
    skeletonRtibia,    skeletonLtibia;

#define    TORSO_SCALE 1.00
#define     HEAD_SCALE 0.25
#define CLAVICLE_SCALE 0.10
#define  HUMERUS_SCALE 0.50
#define   RADIUS_SCALE 0.40
#define   PELVIS_SCALE CLAVICLE_SCALE
#define    FEMUR_SCALE 0.90
#define    TIBIA_SCALE 0.75

#define SKELETON_NECK                 glm::translate(skeletonTorso.model,glm::vec3(0,TORSO_SCALE,0))
#define SKELETON_COLLAR(    rad,axis) glm::rotate(SKELETON_NECK,rad,axis)
#define SKELETON_SHOULDER(m)          glm::translate(m,glm::vec3(0,CLAVICLE_SCALE,0))
#define SKELETON_BICEP(   m,rad,axis) glm::rotate(SKELETON_SHOULDER(m),rad,axis)
#define SKELETON_ELBOW(   m         ) glm::translate(m,glm::vec3(0,HUMERUS_SCALE,0))
#define SKELETON_FOREARM( m,rad,axis) glm::rotate(SKELETON_ELBOW(m),rad,axis)
#define SKELETON_CROTCH               skeletonTorso.model
#define SKELETON_WAIST(     rad,axis) glm::rotate(SKELETON_CROTCH,rad,axis)
#define SKELETON_HIP(     m         ) glm::translate(m,glm::vec3(0,PELVIS_SCALE,0))
#define SKELETON_QUAD(    m,rad,axis) glm::rotate(SKELETON_HIP(m),rad,axis)
#define SKELETON_KNEE(    m         ) glm::translate(m,glm::vec3(0,FEMUR_SCALE,0))
#define SKELETON_CALF(    m,rad,axis) glm::rotate(SKELETON_KNEE(m),rad,axis)

GLfloat
    skeletonRshrug   = rad( 90),
    skeletonRflex    = rad( 45),
    skeletonRsqueeze = rad( 45),
    skeletonLshrug   = rad(-90),
    skeletonLflex    = rad(-45),
    skeletonLsqueeze = rad(-45),
    skeletonBooty    = rad( 90),
    skeletonRlunge   = rad( 75),
    skeletonRkick    = rad( 15),
    skeletonLlunge   = rad(-75),
    skeletonLkick    = rad(-15);

bool skeletonPlay = false;


void drawBone ( glm::mat4 model = glm::mat4() ){

    glUniformMatrix4fv(Cmodel,1,GL_FALSE,glm::value_ptr(model));
    glDrawArrays(GL_LINES,0,2);
};


void buildRarm ( ) {

    skeletonRclavicle.model = glm::mat4();
    skeletonRclavicle.draw  = [](){
        skeletonRclavicle.model = SKELETON_COLLAR(skeletonRshrug,glm::vec3(AXIS_Z));
        drawBone(glm::scale(
            skeletonRclavicle.model,
            glm::vec3(CLAVICLE_SCALE,CLAVICLE_SCALE,CLAVICLE_SCALE)
        ));
    };
    skeletonRclavicle.child   = &skeletonRhumerus;
    skeletonRclavicle.sibling = &skeletonRpelvis;

    skeletonRhumerus.model = glm::mat4();
    skeletonRhumerus.draw  = [](){
        skeletonRhumerus.model = SKELETON_BICEP(skeletonRclavicle.model,skeletonRflex,glm::vec3(AXIS_Z));
        drawBone(glm::scale(
            skeletonRhumerus.model,
            glm::vec3(HUMERUS_SCALE,HUMERUS_SCALE,HUMERUS_SCALE)
        ));
    };
    skeletonRhumerus.child   = &skeletonRradius;
    skeletonRhumerus.sibling = NULL;

    skeletonRradius.model = glm::mat4();
    skeletonRradius.draw  = [](){
        skeletonRradius.model = SKELETON_FOREARM(skeletonRhumerus.model,skeletonRsqueeze,glm::vec3(AXIS_Z));
        drawBone(glm::scale(
            skeletonRradius.model,
            glm::vec3(RADIUS_SCALE,RADIUS_SCALE,RADIUS_SCALE)
        ));
    };
    skeletonRradius.child   = NULL;
    skeletonRradius.sibling = NULL;
}


void buildLarm ( ) {

    skeletonLclavicle.model = glm::mat4();
    skeletonLclavicle.draw  = [](){
        skeletonLclavicle.model = SKELETON_COLLAR(skeletonLshrug,glm::vec3(AXIS_Z));
        drawBone(glm::scale(
            skeletonLclavicle.model,
            glm::vec3(CLAVICLE_SCALE,CLAVICLE_SCALE,CLAVICLE_SCALE)
        ));
    };
    skeletonLclavicle.child   = &skeletonLhumerus;
    skeletonLclavicle.sibling = NULL;

    skeletonLhumerus.model = glm::mat4();
    skeletonLhumerus.draw  = [](){
        skeletonLhumerus.model = SKELETON_BICEP(skeletonLclavicle.model,skeletonLflex,glm::vec3(AXIS_Z));
        drawBone(glm::scale(
            skeletonLhumerus.model,
            glm::vec3(HUMERUS_SCALE,HUMERUS_SCALE,HUMERUS_SCALE)
        ));
    };
    skeletonLhumerus.child   = &skeletonLradius;
    skeletonLhumerus.sibling = NULL;

    skeletonLradius.model = glm::mat4();
    skeletonLradius.draw  = [](){
        skeletonLradius.model = SKELETON_FOREARM(skeletonLhumerus.model,skeletonLsqueeze,glm::vec3(AXIS_Z));
        drawBone(glm::scale(
            skeletonLradius.model,
            glm::vec3(RADIUS_SCALE,RADIUS_SCALE,RADIUS_SCALE)
        ));
    };
    skeletonLradius.child   = NULL;
    skeletonLradius.sibling = NULL;
}


void buildRleg ( ) {

    skeletonRpelvis.model = glm::mat4();
    skeletonRpelvis.draw  = [](){
        skeletonRpelvis.model = SKELETON_WAIST(skeletonBooty,glm::vec3(AXIS_Z));
        drawBone(glm::scale(
            skeletonRpelvis.model,
            glm::vec3(PELVIS_SCALE,PELVIS_SCALE,PELVIS_SCALE)
        ));
    };
    skeletonRpelvis.child   = &skeletonRfemur;
    skeletonRpelvis.sibling = &skeletonLpelvis;

    skeletonRfemur.model = glm::mat4();
    skeletonRfemur.draw  = [](){
        skeletonRfemur.model = SKELETON_QUAD(skeletonRpelvis.model,skeletonRlunge,glm::vec3(AXIS_Z));
        drawBone(glm::scale(
            skeletonRfemur.model,
            glm::vec3(FEMUR_SCALE,FEMUR_SCALE,FEMUR_SCALE)
        ));
    };
    skeletonRfemur.child   = &skeletonRtibia;
    skeletonRfemur.sibling = NULL;

    skeletonRtibia.model = glm::mat4();
    skeletonRtibia.draw  = [](){
        skeletonRtibia.model = SKELETON_CALF(skeletonRfemur.model,skeletonRkick,glm::vec3(AXIS_Z));
        drawBone(glm::scale(
            skeletonRtibia.model,
            glm::vec3(TIBIA_SCALE,TIBIA_SCALE,TIBIA_SCALE)
        ));
    };
    skeletonRtibia.child   = NULL;
    skeletonRtibia.sibling = NULL;
}


void buildLleg ( ) {

    skeletonLpelvis.model = glm::mat4();
    skeletonLpelvis.draw  = [](){
        skeletonLpelvis.model = SKELETON_WAIST((GLfloat) (skeletonBooty-rad(180)),glm::vec3(AXIS_Z));
        drawBone(glm::scale(
            skeletonLpelvis.model,
            glm::vec3(PELVIS_SCALE,PELVIS_SCALE,PELVIS_SCALE)
        ));
    };
    skeletonLpelvis.child   = &skeletonLfemur;
    skeletonLpelvis.sibling = &skeletonLclavicle;

    skeletonLfemur.model = glm::mat4();
    skeletonLfemur.draw  = [](){
        skeletonLfemur.model = SKELETON_QUAD(skeletonLpelvis.model,skeletonLlunge,glm::vec3(AXIS_Z));
        drawBone(glm::scale(
            skeletonLfemur.model,
            glm::vec3(FEMUR_SCALE,FEMUR_SCALE,FEMUR_SCALE)
        ));
    };
    skeletonLfemur.child   = &skeletonLtibia;
    skeletonLfemur.sibling = NULL;

    skeletonLtibia.model = glm::mat4();
    skeletonLtibia.draw  = [](){
        skeletonLtibia.model = SKELETON_CALF(skeletonLfemur.model,skeletonLkick,glm::vec3(AXIS_Z));
        drawBone(glm::scale(
            skeletonLtibia.model,
            glm::vec3(TIBIA_SCALE,TIBIA_SCALE,TIBIA_SCALE)
        ));
    };
    skeletonLtibia.child   = NULL;
    skeletonLtibia.sibling = NULL;
}


void buildSkeleton ( ) {

    skeletonTorso.model = glm::scale(
        glm::mat4(),
        glm::vec3(TORSO_SCALE,TORSO_SCALE,TORSO_SCALE)
    );
    skeletonTorso.draw = [](){ drawBone(skeletonTorso.model); };
    skeletonTorso.child   = &skeletonHead;
    skeletonTorso.sibling = NULL;

    skeletonHead.model = glm::mat4();
    skeletonHead.draw  = [](){
        skeletonHead.model = SKELETON_NECK;
        drawBone(glm::scale(
            skeletonHead.model,
            glm::vec3(HEAD_SCALE,HEAD_SCALE,HEAD_SCALE)
        ));
    };
    skeletonHead.child   = NULL;
    skeletonHead.sibling = &skeletonRclavicle;

    buildLarm(); buildLleg();
    buildRarm(); buildRleg();
}


void initSkeleton ( ) {
    static bool uninitialized = true;
    if (uninitialized) {

        glm::vec4 * const points = new glm::vec4[2];
        glm::vec4 * const colors = new glm::vec4[2];

        buildSkeleton();
        points[0] = AXIS_ORIGIN;        colors[0] = palette[BASIC_WHITE];
        points[1] = glm::vec4(0,1,0,1); colors[1] = palette[BASIC_WHITE];

        Cobject(
            &boneBuffer,
            2,
            points,
            colors
        );

        delete [] colors;
        delete [] points;
    }
    else cleanCobject(&boneBuffer);
    uninitialized = not uninitialized;
}


void recursiveSkeleton ( bone_t * root ) {

    if (root == NULL) return;
        root->draw();
    if (root->child   != NULL) recursiveSkeleton(root->child);
    if (root->sibling != NULL) recursiveSkeleton(root->sibling);
}


void drawSkeleton ( glm::mat4 model = glm::mat4() ) {

    currentShader(Cshader);
    glBindVertexArray(boneBuffer.VAO);

    GLfloat legShift = 0;
    legShift += glm::max(
        glm::abs(glm::sin(rad(skeletonLlunge)))*FEMUR_SCALE,
        glm::abs(glm::sin(rad(skeletonRlunge)))*FEMUR_SCALE
    );
    legShift += glm::max(
        glm::abs(glm::sin(rad(skeletonLlunge+skeletonLkick)))*TIBIA_SCALE,
        glm::abs(glm::sin(rad(skeletonRlunge+skeletonRkick)))*TIBIA_SCALE
    );
    model = glm::translate(model,glm::normalize(glm::vec3(vUp))*legShift);

    glm::vec4 pSkeleton = model*AXIS_ORIGIN;
    skeletonTorso.model = glm::translate(model,glm::vec3(0,RED(PIXEL(terrainCanvas,(GLint) (pSkeleton.x),(GLint) (pSkeleton.z))),0));

//    pSkeleton = model*AXIS_ORIGIN;
//    if (pSkeleton.x <  0)                    pSkeleton.x = 0;
//    if (pSkeleton.x >= terrainCanvas.width)  pSkeleton.x = terrainCanvas.width-1;
//    if (pSkeleton.z <  0)                    pSkeleton.z = 0;
//    if (pSkeleton.z >= terrainCanvas.height) pSkeleton.z = terrainCanvas.height-1;

    if (skeletonPlay) {

        GLfloat variation = sin(rad(glfwGetTime()*300));

        skeletonRshrug    =  (variation*rad(  5))+rad(90),
        skeletonRflex     =  (variation*rad( 80)),
        skeletonRsqueeze  =  (variation*rad( 10)),
        skeletonLshrug    =  (variation*rad(- 5))-rad(90),
        skeletonLflex     =  (variation*rad(-80)),
        skeletonLsqueeze  =  (variation*rad(-10)),
        skeletonRlunge    =  (variation*rad( 15))+rad(75),
        skeletonRkick     =  (variation*rad(- 5))+rad( 5),
        skeletonLlunge    =  (variation*rad(-15))-rad(75),
        skeletonLkick     =  (variation*rad(  5))-rad( 5);
    }

    recursiveSkeleton(&skeletonTorso);
}
