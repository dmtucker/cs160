#ifndef COLOR_H
#define COLOR_H

#include <glm/glm.hpp>

#define BASIC_BLACK   0
#define BASIC_BLUE    1
#define BASIC_GREEN   2
#define BASIC_CYAN    3
#define BASIC_RED     4
#define BASIC_MAGENTA 5
#define BASIC_YELLOW  6
#define BASIC_WHITE   7

#define GRAY_LIGHTEST 8
#define GRAY_LIGHTER  9
#define GRAY_LIGHT    10
#define GRAY          11
#define GRAY_DARK     12
#define GRAY_DARKER   13
#define GRAY_DARKEST  14

#define CSS_AQUA      15
#define CSS_BLACK     16
#define CSS_BLUE      17
#define CSS_FUCHSIA   18
#define CSS_GRAY      19
#define CSS_GREEN     20
#define CSS_LIME      21
#define CSS_MAROON    22
#define CSS_NAVY      23
#define CSS_OLIVE     24
#define CSS_ORANGE    25
#define CSS_PURPLE    26
#define CSS_RED       27
#define CSS_SILVER    28
#define CSS_TEAL      29
#define CSS_WHITE     30
#define CSS_YELLOW    31

const glm::vec4 palette[32] = {

    // (8) basic
    glm::vec4(0.000,0.000,0.000,1.000), // black
    glm::vec4(0.000,0.000,1.000,1.000), // blue
    glm::vec4(0.000,1.000,0.000,1.000), // green
    glm::vec4(0.000,1.000,1.000,1.000), // cyan
    glm::vec4(1.000,0.000,0.000,1.000), // red
    glm::vec4(1.000,0.000,1.000,1.000), // magenta
    glm::vec4(1.000,1.000,0.000,1.000), // yellow
    glm::vec4(1.000,1.000,1.000,1.000), // white

    // (7) grayscale
    glm::vec4(0.875,0.875,0.875,1.000), // lightest gray
    glm::vec4(0.750,0.750,0.750,1.000), // lighter gray
    glm::vec4(0.625,0.625,0.625,1.000), // light gray
    glm::vec4(0.500,0.500,0.500,1.000), // gray
    glm::vec4(0.375,0.375,0.375,1.000), // dark gray
    glm::vec4(0.250,0.250,0.250,1.000), // darker gray
    glm::vec4(0.125,0.125,0.125,1.000), // darkest gray

    // (17) CSS standard
    glm::vec4(0.000,1.000,1.000,1.000), // aqua
    glm::vec4(0.000,0.000,0.000,1.000), // black
    glm::vec4(0.000,0.000,1.000,1.000), // blue
    glm::vec4(1.000,0.000,1.000,1.000), // fuchsia
    glm::vec4(0.502,0.502,0.502,1.000), // gray
    glm::vec4(0.000,0.502,0.000,1.000), // green
    glm::vec4(0.000,1.000,0.000,1.000), // lime
    glm::vec4(0.502,0.000,0.000,1.000), // maroon
    glm::vec4(0.000,0.000,0.502,1.000), // navy
    glm::vec4(0.502,0.502,0.000,1.000), // olive
    glm::vec4(1.000,0.647,0.000,1.000), // orange
    glm::vec4(0.502,0.000,0.502,1.000), // purple
    glm::vec4(1.000,0.000,0.000,1.000), // red
    glm::vec4(0.753,0.753,0.753,1.000), // silver
    glm::vec4(0.000,0.502,0.502,1.000), // teal
    glm::vec4(1.000,1.000,1.000,1.000), // white
    glm::vec4(1.000,1.000,0.000,1.000), // yellow
};

unsigned int brushColor = BASIC_WHITE;

#endif
