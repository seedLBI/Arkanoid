#include "Arkanoid_Math.h"


float getTimeCollisionBetweenTwoSegment(const Segment& A, const Segment& B) {
	float numerator = (A.begin.x - B.begin.x) * (B.begin.y - B.end.y) - (A.begin.y - B.begin.y) * (B.begin.x - B.end.x);
	float denumerator = (A.begin.x - A.end.x) * (B.begin.y - B.end.y) - (A.begin.y - A.end.y) * (B.begin.x - B.begin.y);

	return numerator / denumerator;
}

glm::vec2 lerp(const Segment& s, const float& time) {
	return s.begin + time * s.getDirection();
}

glm::vec2 lerp(const glm::vec2& A, const glm::vec2& B, const float& time) {
	return A + time * (B - A);
}

float cross2d(glm::vec2 a, glm::vec2 b) {
	return a.x * b.y - a.y * b.x;
}

float angleBetweenVectors(const glm::vec2& dir1, const glm::vec2& dir2) {
	return std::acosf(glm::dot(dir1, dir2) / (glm::length(dir1) * glm::length(dir2)));
}

glm::vec2 rotate(const glm::vec2& dir1, const float& angle) {
	const glm::mat2 rot_mat = {
		{cosf(angle),-sinf(angle)},
		{sinf(angle), cosf(angle)}
	};
	return dir1 * rot_mat;
}
