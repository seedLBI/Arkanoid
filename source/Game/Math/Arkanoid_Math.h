#ifndef ARKANOID_MATH_H
#define ARKANOID_MATH_H

#include "Game/Segment/Segment.h"
#include <glm/glm.hpp>

float getTimeCollisionBetweenTwoSegment(const Segment& A, const Segment& B);
glm::vec2 lerp(const Segment& s, const float& time);
glm::vec2 lerp(const glm::vec2& A, const glm::vec2& B, const float& time);

float cross2d(glm::vec2 a, glm::vec2 b);

float angleBetweenVectors(const glm::vec2& dir1, const glm::vec2& dir2);
float atan2(const glm::vec2& dir);

glm::vec2 getPointOnCircle(const float& angle);


glm::vec2 rotate(const glm::vec2& dir1, const float& angle);

glm::vec2 perp_normalized(const glm::vec2& p);

#endif