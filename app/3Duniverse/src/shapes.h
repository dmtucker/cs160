#ifndef SHAPES_H
#define SHAPES_H

#include <glm/glm.hpp>

#define AXIS_X      vec4(1,0,0,0)
#define AXIS_Y      vec4(0,1,0,0)
#define AXIS_Z      vec4(0,0,1,0)
#define AXIS_ORIGIN vec4(0,0,0,1)

const glm::vec4 cube[36] = {
    
    // back
    vec4(-1,-1,-1, 1),
    vec4(-1, 1,-1, 1),
    vec4( 1, 1,-1, 1),
    vec4( 1, 1,-1, 1),
    vec4( 1,-1,-1, 1),
    vec4(-1,-1,-1, 1),
    
    // bottom
    vec4(-1,-1,-1, 1),
    vec4( 1,-1,-1, 1),
    vec4( 1,-1, 1, 1),
    vec4( 1,-1, 1, 1),
    vec4(-1,-1, 1, 1),
    vec4(-1,-1,-1, 1),
    
    // left
    vec4(-1,-1,-1, 1),
    vec4(-1,-1, 1, 1),
    vec4(-1, 1, 1, 1),
    vec4(-1, 1, 1, 1),
    vec4(-1, 1,-1, 1),
    vec4(-1,-1,-1, 1),
    
    // right
    vec4( 1, 1, 1, 1),
    vec4( 1,-1, 1, 1),
    vec4( 1,-1,-1, 1),
    vec4( 1,-1,-1, 1),
    vec4( 1, 1,-1, 1),
    vec4( 1, 1, 1, 1),
    
    // top
    vec4( 1, 1, 1, 1),
    vec4( 1, 1,-1, 1),
    vec4(-1, 1,-1, 1),
    vec4(-1, 1,-1, 1),
    vec4(-1, 1, 1, 1),
    vec4( 1, 1, 1, 1),
    
    // front
    vec4( 1, 1, 1, 1),
    vec4(-1, 1, 1, 1),
    vec4(-1,-1, 1, 1),
    vec4(-1,-1, 1, 1),
    vec4( 1,-1, 1, 1),
    vec4( 1, 1, 1, 1)
};

#endif
