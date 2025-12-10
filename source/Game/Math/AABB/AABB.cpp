#include "AABB.h"

#include <iostream>

AABB_Region GetAABB(const std::vector<glm::vec2>& vertices) {
	AABB_Region output;

	output.min =  glm::vec2(100000.f, 100000.f);
	output.max = -glm::vec2(100000.f, 100000.f);

	for (size_t i = 0; i < vertices.size(); i++) {
		const glm::vec2& point_check = vertices[i];

		output.min.x = (std::min)(point_check.x, output.min.x);
		output.max.x = (std::max)(point_check.x, output.max.x);

		output.min.y = (std::min)(point_check.y, output.min.y);
		output.max.y = (std::max)(point_check.y, output.max.y);

	}

	return output;
}

bool Collision_Point_and_AABB(const glm::vec2& point, const AABB_Region& region) {
	return point.x >= region.min.x && point.x <= region.max.x &&
		point.y >= region.min.y && point.y <= region.max.y;
}

bool Collision_Circle_and_AABB(const glm::vec2& circle_pos, const float& radius, const AABB_Region& region) {
	const float x = (std::max)(region.min.x, (std::min)(circle_pos.x, region.max.x));
	const float y = (std::max)(region.min.y, (std::min)(circle_pos.y, region.max.y));

	glm::vec2 pos_check(x, y);

	return glm::length(circle_pos - pos_check) < radius;
}

bool Collision_AABB_and_AABB(const AABB_Region& A, const AABB_Region& B) {
	return (A.min.x <= B.max.x) && (A.max.x >= B.min.x) &&
		(A.min.y <= B.max.y) && (A.max.y >= B.min.y);
}