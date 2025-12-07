#ifndef MAIN_BLOCK_H
#define MAIN_BLOCK_H

#include <glm/glm.hpp>

struct MainBlock{
    glm::mat4 projectionView;
    glm::vec2 Resolution;
    float timeProgramm;
    float deltaTime;
};

#endif
