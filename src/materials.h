#ifndef MATERIALS_H
#define MATERIALS_H

#include <glm/glm.hpp>

const float     brass_shininess = 27.8;
const glm::vec4 brass_ambient   = glm::vec4(0.33,0.22,0.03,1.00),
                brass_diffuse   = glm::vec4(0.78,0.57,0.11,1.00),
                brass_specular  = glm::vec4(0.99,0.91,0.81,1.00);

const float     bright_shininess = 1;
const glm::vec4 bright_ambient   = glm::vec4(1,1,1,1),
                bright_diffuse   = glm::vec4(1,1,1,1),
                bright_specular  = glm::vec4(1,1,1,1);

#endif
