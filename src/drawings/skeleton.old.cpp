GPUbuffer boneBuffer;

typedef struct bone_t {
    glm::mat4 model;
    void (*draw)( glm::mat4 model );
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


void drawBone ( glm::mat4 model = glm::mat4() ){
    
    glUniformMatrix4fv(Cmodel,1,GL_FALSE,glm::value_ptr(model));
    glDrawArrays(GL_LINES,0,2);
};


void buildSkeleton ( ) {
    
    skeletonTorso.model   = glm::mat4();
    skeletonTorso.model   = glm::scale(skeletonTorso.model,glm::vec3(TORSO_SCALE,TORSO_SCALE,TORSO_SCALE));
    skeletonTorso.draw    = []( glm::mat4 model = glm::mat4() ){ drawBone(model); };
    skeletonTorso.child   = &skeletonHead;
    skeletonTorso.sibling = NULL;
    
    glm::mat4 neck      = glm::translate(glm::mat4(),glm::vec3(0,TORSO_SCALE,0));
    glm::mat4 Rcollar   = glm::rotate   (neck       , 90.0f,glm::vec3(AXIS_Z));
    glm::mat4 Lcollar   = glm::rotate   (neck       ,-90.0f,glm::vec3(AXIS_Z));
    glm::mat4 Rshoulder = glm::translate(Rcollar    ,glm::vec3(0,CLAVICLE_SCALE,0));
    glm::mat4 Lshoulder = glm::translate(Lcollar    ,glm::vec3(0,CLAVICLE_SCALE,0));
    glm::mat4 Rbicep    = glm::rotate   (Rshoulder  , 45.0f,glm::vec3(AXIS_Z));
    glm::mat4 Lbicep    = glm::rotate   (Lshoulder  ,-45.0f,glm::vec3(AXIS_Z));
    glm::mat4 Relbow    = glm::translate(Rbicep     ,glm::vec3(0,HUMERUS_SCALE,0));
    glm::mat4 Lelbow    = glm::translate(Lbicep     ,glm::vec3(0,HUMERUS_SCALE,0));
    glm::mat4 Rforearm  = glm::rotate   (Relbow     , 45.0f,glm::vec3(AXIS_Z));
    glm::mat4 Lforearm  = glm::rotate   (Lelbow     ,-45.0f,glm::vec3(AXIS_Z));
    glm::mat4 crotch    = glm::mat4();
    glm::mat4 Rwaist    = glm::rotate   (crotch     , 90.0f,glm::vec3(AXIS_Z));
    glm::mat4 Lwaist    = glm::rotate   (crotch     ,-90.0f,glm::vec3(AXIS_Z));
    glm::mat4 Rhip      = glm::translate(Rwaist     ,glm::vec3(0,PELVIS_SCALE,0));
    glm::mat4 Lhip      = glm::translate(Lwaist     ,glm::vec3(0,PELVIS_SCALE,0));
    glm::mat4 Rquad     = glm::rotate   (Rhip       , 75.0f,glm::vec3(AXIS_Z));
    glm::mat4 Lquad     = glm::rotate   (Lhip       ,-75.0f,glm::vec3(AXIS_Z));
    glm::mat4 Rknee     = glm::translate(Rquad      ,glm::vec3(0,FEMUR_SCALE,0));
    glm::mat4 Lknee     = glm::translate(Lquad      ,glm::vec3(0,FEMUR_SCALE,0));
    glm::mat4 Rcalf     = glm::rotate   (Rknee      , 15.0f,glm::vec3(AXIS_Z));
    glm::mat4 Lcalf     = glm::rotate   (Lknee      ,-15.0f,glm::vec3(AXIS_Z));
    
    // head
    skeletonHead.model   = neck;
    skeletonHead.model   = glm::scale(skeletonHead.model,glm::vec3(HEAD_SCALE,HEAD_SCALE,HEAD_SCALE));
    skeletonHead.draw    = []( glm::mat4 model = glm::mat4() ){ drawBone(model); };
    skeletonHead.child   = NULL;
    skeletonHead.sibling = &skeletonRclavicle;
    
    // right arm
    skeletonRclavicle.model   = Rcollar;
    skeletonRclavicle.model   = glm::scale(skeletonRclavicle.model,glm::vec3(CLAVICLE_SCALE,CLAVICLE_SCALE,CLAVICLE_SCALE));
    skeletonRclavicle.draw    = []( glm::mat4 model = glm::mat4() ){ drawBone(model); };
    skeletonRclavicle.child   = &skeletonRhumerus;
    skeletonRclavicle.sibling = &skeletonRpelvis;
    
    skeletonRhumerus.model   = Rbicep;
    skeletonRhumerus.model   = glm::scale(skeletonRhumerus.model,glm::vec3(HUMERUS_SCALE,HUMERUS_SCALE,HUMERUS_SCALE));
    skeletonRhumerus.draw    = []( glm::mat4 model = glm::mat4() ){ drawBone(model); };
    skeletonRhumerus.child   = &skeletonRradius;
    skeletonRhumerus.sibling = NULL;
    
    skeletonRradius.model   = Rforearm;
    skeletonRradius.model   = glm::scale(skeletonRradius.model,glm::vec3(RADIUS_SCALE,RADIUS_SCALE,RADIUS_SCALE));
    skeletonRradius.draw    = []( glm::mat4 model = glm::mat4() ){ drawBone(model); };
    skeletonRradius.child   = NULL;
    skeletonRradius.sibling = NULL;
    
    // right leg
    skeletonRpelvis.model   = Rwaist;
    skeletonRpelvis.model   = glm::scale(skeletonRpelvis.model,glm::vec3(PELVIS_SCALE,PELVIS_SCALE,PELVIS_SCALE));
    skeletonRpelvis.draw    = []( glm::mat4 model = glm::mat4() ){ drawBone(model); };
    skeletonRpelvis.child   = &skeletonRfemur;
    skeletonRpelvis.sibling = &skeletonLpelvis;
    
    skeletonRfemur.model   = Rquad;
    skeletonRfemur.model   = glm::scale(skeletonRfemur.model,glm::vec3(FEMUR_SCALE,FEMUR_SCALE,FEMUR_SCALE));
    skeletonRfemur.draw    = []( glm::mat4 model = glm::mat4() ){ drawBone(model); };
    skeletonRfemur.child   = &skeletonRtibia;
    skeletonRfemur.sibling = NULL;
    
    skeletonRtibia.model   = Rcalf;
    skeletonRtibia.model   = glm::scale(skeletonRtibia.model,glm::vec3(TIBIA_SCALE,TIBIA_SCALE,TIBIA_SCALE));
    skeletonRtibia.draw    = []( glm::mat4 model = glm::mat4() ){ drawBone(model); };
    skeletonRtibia.child   = NULL;
    skeletonRtibia.sibling = NULL;
    
    // left leg
    skeletonLpelvis.model   = Lwaist;
    skeletonLpelvis.model   = glm::scale(skeletonLpelvis.model,glm::vec3(PELVIS_SCALE,PELVIS_SCALE,PELVIS_SCALE));
    skeletonLpelvis.draw    = []( glm::mat4 model = glm::mat4() ){ drawBone(model); };
    skeletonLpelvis.child   = &skeletonLfemur;
    skeletonLpelvis.sibling = &skeletonLclavicle;
    
    skeletonLfemur.model   = Lquad;
    skeletonLfemur.model   = glm::scale(skeletonLfemur.model,glm::vec3(FEMUR_SCALE,FEMUR_SCALE,FEMUR_SCALE));
    skeletonLfemur.draw    = []( glm::mat4 model = glm::mat4() ){ drawBone(model); };
    skeletonLfemur.child   = &skeletonLtibia;
    skeletonLfemur.sibling = NULL;
    
    skeletonLtibia.model   = Lcalf;
    skeletonLtibia.model   = glm::scale(skeletonLtibia.model,glm::vec3(TIBIA_SCALE,TIBIA_SCALE,TIBIA_SCALE));
    skeletonLtibia.draw    = []( glm::mat4 model = glm::mat4() ){ drawBone(model); };
    skeletonLtibia.child   = NULL;
    skeletonLtibia.sibling = NULL;
    
    // left arm
    skeletonLclavicle.model   = Lcollar;
    skeletonLclavicle.model   = glm::scale(skeletonLclavicle.model,glm::vec3(CLAVICLE_SCALE,CLAVICLE_SCALE,CLAVICLE_SCALE));
    skeletonLclavicle.draw    = []( glm::mat4 model = glm::mat4() ){ drawBone(model); };
    skeletonLclavicle.child   = &skeletonLhumerus;
    skeletonLclavicle.sibling = NULL;
    
    skeletonLhumerus.model   = Lbicep;
    skeletonLhumerus.model   = glm::scale(skeletonLhumerus.model,glm::vec3(HUMERUS_SCALE,HUMERUS_SCALE,HUMERUS_SCALE));
    skeletonLhumerus.draw    = []( glm::mat4 model = glm::mat4() ){ drawBone(model); };
    skeletonLhumerus.child   = &skeletonLradius;
    skeletonLhumerus.sibling = NULL;
    
    skeletonLradius.model   = Lforearm;
    skeletonLradius.model   = glm::scale(skeletonLradius.model,glm::vec3(RADIUS_SCALE,RADIUS_SCALE,RADIUS_SCALE));
    skeletonLradius.draw    = []( glm::mat4 model = glm::mat4() ){ drawBone(model); };
    skeletonLradius.child   = NULL;
    skeletonLradius.sibling = NULL;
}


void initSkeleton ( ) {
    static bool uninitialized = true;
    if (uninitialized) {
        
        glm::vec4 * const points = new glm::vec4[2];
        glm::vec4 * const colors = new glm::vec4[2];
        
        buildSkeleton();
        points[0] = AXIS_ORIGIN;
        points[1] = glm::vec4(0,1,0,1);
        colors[0] = palette[BASIC_BLUE]; //XXX
        colors[1] = palette[BASIC_WHITE];
        
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
    root->draw(root->model);
    if (root->child   != NULL) recursiveSkeleton(root->child);
    if (root->sibling != NULL) recursiveSkeleton(root->sibling);
}


void drawSkeleton ( glm::mat4 model = glm::mat4() ) {
    
    currentShader(Cshader);
    glBindVertexArray(boneBuffer.VAO);
    
    recursiveSkeleton(&skeletonTorso);
}
