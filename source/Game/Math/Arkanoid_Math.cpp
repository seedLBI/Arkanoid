#include "Arkanoid_Math.h"
#include <glm/gtc/constants.hpp>

float getTimeCollisionBetweenTwoSegment(const Segment& A, const Segment& B) {

	const float& D1x = A.begin.x;
	const float& D1y = A.begin.y;
	const float& D2x = A.end.x;
	const float& D2y = A.end.y;

	const float& D3x = B.begin.x;
	const float& D3y = B.begin.y;
	const float& D4x = B.end.x;
	const float& D4y = B.end.y;

	float numerator   = (D1x - D3x) * (D3y - D4y) - (D1y - D3y) * (D3x - D4x);
	float denumerator = (D1x - D2x) * (D3y - D4y) - (D1y - D2y) * (D3x - D4x);

	//float numerator =   (A.begin.x - B.begin.x) * (B.begin.y - B.end.y) - (A.begin.y - B.begin.y) * (B.begin.x - B.end.x);
	//float denumerator = (A.begin.x - A.end.x)   * (B.begin.y - B.end.y) - (A.begin.y - A.end.y)   * (B.begin.x - B.end.x);

	return numerator / denumerator;
}

glm::vec2 lerp(const Segment& s, const float& time) {
	return s.begin + time * (s.end - s.begin);
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


float atan2(const glm::vec2& dir) {
	return std::atan2f(dir.y,dir.x);
}


glm::vec2 getPointOnCircle(const float& angle) {
	return glm::vec2{ std::cosf(angle), std::sinf(angle) };
}


glm::vec2 rotate(const glm::vec2& dir1, const float& angle) {
	const glm::mat2 rot_mat = {
		{cosf(angle),-sinf(angle)},
		{sinf(angle), cosf(angle)}
	};
	return rot_mat * dir1;
}

glm::vec2 perp_normalized(const glm::vec2& p) {
	return glm::normalize(glm::vec2{ -p.y,p.x });
}
