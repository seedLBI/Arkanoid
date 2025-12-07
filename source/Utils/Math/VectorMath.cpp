#include "VectorMath.h"

float  dotProd(const glm::vec2& v1, const glm::vec2& v2) { return v1.x * v2.x + v1.y * v2.y; }
float  dotProd(const glm::vec2& v) { return dotProd(v, v); }

inline float  projOfVecMul(const glm::vec2& v1, const glm::vec2& v2) { return v1.x * v2.y - v1.y * v2.x; }

bool pointInTriangle(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& p, float eps) {
    float c1 = projOfVecMul(b - a, p - a);
    float c2 = projOfVecMul(c - b, p - b);
    float c3 = projOfVecMul(a - c, p - c);

    bool hasNeg = (c1 < -eps) || (c2 < -eps) || (c3 < -eps);
    bool hasPos = (c1 > eps) || (c2 > eps) || (c3 > eps);
    return !(hasNeg && hasPos);
}

bool pointInCircle(const glm::vec2& pos, const float& radius, const glm::vec2& point) {
    float delta = dotProd(pos - point);
    return sqrtf(delta) <= radius;
}