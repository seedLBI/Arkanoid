#include "LayerBorder.h"

#include "Game/Segment/Segment.h"
#include "Game/Math/Arkanoid_Math.h"
#include "Core/CoordinateSystem/CoordinateSystem.h"

#include <algorithm>

LayerBorder::LayerBorder(const std::vector<glm::vec2>& vertices) {
	this->vertices = vertices;
}

LayerBorder::~LayerBorder() {

}

void LayerBorder::Draw(DebugLine& renderer) {
	for (size_t i = 0; i < vertices.size() - 1; i++)
		renderer.Add(vertices[i], vertices[i + 1], 13.f, color_borders, TranslateGlobalToScreen);
}

std::optional<CollisionInfo> LayerBorder::GetCollision(const glm::vec2& begin, const glm::vec2& end) {
	const Segment ball_path{ begin,end };

	std::vector<std::pair<size_t,glm::vec2>> collisions;

	for (size_t i = 0; i < vertices.size() - 1; i++) {
		Segment border{ vertices[i],vertices[i + 1] };

		float time = getTimeCollisionBetweenTwoSegment(ball_path, border);

		if (time > 1.f || time < 0.f)
			continue;

		glm::vec2 point = lerp(ball_path, time);

		collisions.push_back({i, point });
	}

	if (collisions.empty())
		return std::nullopt;
	

	std::sort(collisions.begin(), collisions.end(), [&begin](const auto& a, const auto& b) {
		return glm::length(a.second - begin) > glm::length(b.second - begin);
		});

	const glm::vec2& nereast_collision = collisions.back().second;
	const size_t& index = collisions.back().first;

	glm::vec2 border_dir = Segment{ vertices[index],vertices[index + 1] }.getDirection();
	glm::vec2 ball_dir = ball_path.getDirection();

	if (cross2d(border_dir,ball_dir) > 0.f)
		ball_dir *= -1.f;

	float angle = angleBetweenVectors(ball_dir, border_dir);
	glm::vec2 tangent_bounce = rotate(glm::normalize(border_dir), angle - glm::pi<float>());

	CollisionInfo output;
	output.position = nereast_collision;
	output.tangentBound = tangent_bounce;

	return output;
}
