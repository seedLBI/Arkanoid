#ifndef VECTOR_MATH_H
#define VECTOR_MATH_H
#include <glm/glm.hpp>

float  dotProd(const glm::vec2& v1, const glm::vec2& v2);
float  dotProd(const glm::vec2& v);

inline float projOfVecMul(const glm::vec2& v1, const glm::vec2& v2);

bool pointInTriangle(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& p, float eps = 1e-6f);
bool pointInCircle(const glm::vec2& pos, const float& radius, const glm::vec2& point);


#endif