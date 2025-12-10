#ifndef ARKANOID_MATH_H
#define ARKANOID_MATH_H

#include "Game/Math/Segment/Segment.h"
#include "Game/CollisionInfo/CollisionInfo.h"
#include <glm/glm.hpp>
#include <vector>
#include <optional>
#include <algorithm>

float getTimeCollisionBetweenTwoSegment(const Segment& A, const Segment& B);
glm::vec2 lerp(const Segment& s, const float& time);
glm::vec2 lerp(const glm::vec2& A, const glm::vec2& B, const float& time);

float cross2d(glm::vec2 a, glm::vec2 b);

float angleBetweenVectors(const glm::vec2& dir1, const glm::vec2& dir2);
float atan2(const glm::vec2& dir);

glm::vec2 getPointOnCircle(const float& angle);

bool isIntersectPointCircle(const glm::vec2& point,const glm::vec2& posCircle, const float& radius);

glm::vec2 rotate(const glm::vec2& dir1, const float& angle);

glm::vec2 perp_normalized(const glm::vec2& p);

bool isClockwise(const std::vector<glm::vec2>& polygon, bool yAxisUp = true);


std::optional<CollisionInfo> GetCollision(const std::vector<glm::vec2>& border_vertices, const glm::vec2& begin, const glm::vec2& end);
std::vector<glm::vec2> GenerateRadiusBorder(const std::vector<glm::vec2>& original_border, const float& radius, const bool& outer_border);


#endif