#include "LevelBorder.h"

#include "Game/Segment/Segment.h"
#include "Game/Math/Arkanoid_Math.h"
#include "Core/CoordinateSystem/CoordinateSystem.h"

#include "Core/Engine/Engine.h"

#include <algorithm>

LevelBorder::LevelBorder(const std::vector<glm::vec2>& vertices) {
	this->vertices = vertices;

	if (this->vertices.front() != this->vertices.back()) {
		this->vertices.push_back(this->vertices.front());
	}


	this->vertices_outer = GenerateRadiusBorder(0.05f, true);
	this->vertices_inner = GenerateRadiusBorder(0.05f, false);
}

LevelBorder::~LevelBorder() {

}

const std::vector<glm::vec2>& LevelBorder::GetVertices() {
	return vertices;
}


void LevelBorder::Draw(DebugLine& renderer) {
	for (size_t i = 0; i < vertices.size() - 1; i++)
		renderer.Add(vertices[i], vertices[i + 1], 3.f, color_borders, TranslateGlobalToScreen);

	for (size_t i = 0; i < vertices_outer.size() - 1; i++)
		renderer.Add(vertices_outer[i], vertices_outer[i + 1], 8.f, glm::vec4(1.f,0.2f,0.2f,1.f), TranslateGlobalToScreen);

	for (size_t i = 0; i < vertices_inner.size() - 1; i++)
		renderer.Add(vertices_inner[i], vertices_inner[i + 1], 8.f, glm::vec4(0.2f, 1.f, 0.2f, 1.f), TranslateGlobalToScreen);
}


std::vector<glm::vec2> LevelBorder::GenerateRadiusBorder(const float& radius, const bool& outer_border) {

	std::vector<glm::vec2> output;

	for (size_t i = 1; i < vertices.size(); i++) {

		Segment A, B;

		if (i == vertices.size() - 1) {
			A = { vertices[i-1], vertices[i] };
			B = { vertices[i], vertices[1] };
		}
		else {
			A = { vertices[i - 1], vertices[i] };
			B = { vertices[i], vertices[i + 1] };
		}


		
		glm::vec2 dirA = A.begin - A.end;
		glm::vec2 dirB = B.begin - B.end;

		float sign_radius = outer_border == true ? -1.f: 1.f;

		glm::vec2 perpA = perp_normalized(dirA) * radius;
		glm::vec2 perpB = perp_normalized(dirB) * radius;

		perpA *= sign_radius;
		perpB *= sign_radius;

		Segment A_radius{ A.begin + perpA, A.end + perpA };
		Segment B_radius{ B.begin + perpB, B.end + perpB };
		
		glm::vec2 dirA_radius = A_radius.getDirection();
		glm::vec2 dirB_radius = B_radius.getDirection();

		bool circle_moment = cross2d(dirA_radius, dirB_radius) * (- 1.f) * sign_radius > 0 ? false : true;

		if (circle_moment) {
			float sign = cross2d(dirA, dirB) > 0.f ? 1.f : 3.f;

			const float& PI = glm::pi<float>();

			float alphaA = atan2(dirA);
			float alphaB = atan2(dirB);

			float angleA = alphaA + PI / 2.f * sign;
			float angleB = alphaB + PI / 2.f * sign;

			float diff = angleB - angleA;
			diff = atan2({ cosf(diff), sinf(diff)  });

			int count_samples = std::floor((std::abs(diff) / PI) * MaxSampleRateCircle);

			for (size_t sample = 0; sample <= count_samples; sample++) {

				float time = (float)sample / (float)count_samples;
				float angle_interpolated = angleA + time * diff;

				glm::vec2 point_circle = A.end + getPointOnCircle(angle_interpolated) * radius;

				output.push_back(point_circle);
			}


		}
		else {
			float t = getTimeCollisionBetweenTwoSegment(A_radius, B_radius);

			glm::vec2 point = lerp(A_radius, t);

			output.push_back(point);

		}

	}


	output.push_back(output.front());

	return output;
}

std::optional<CollisionInfo> LevelBorder::GetCollision(const glm::vec2& begin, const glm::vec2& end) {
	const Segment ball_path{ begin,end };

	std::vector<std::pair<size_t,glm::vec2>> collisions;
	

	for (size_t i = 0; i < vertices_inner.size() - 1; i++) {
		Segment border{ vertices_inner[i], vertices_inner[i + 1] };

		float time = getTimeCollisionBetweenTwoSegment(border, ball_path);
		float time2 = getTimeCollisionBetweenTwoSegment(ball_path, border);

		if (time > 1.f || time < 0.f || time2 > 1.f || time2 < 0.f)
			continue;

		glm::vec2 point = lerp(border, time);
		collisions.push_back({i, point });
	}

	if (collisions.empty())
		return std::nullopt;
	
	std::sort(collisions.begin(), collisions.end(), [&begin](const auto& a, const auto& b) {
		return glm::length(a.second - begin) > glm::length(b.second - begin);
	});

	const auto& nereast_collision = collisions.back();

	const glm::vec2& point_collision = nereast_collision.second;
	const size_t& index = nereast_collision.first;

	glm::vec2 border_dir = Segment{ vertices_inner[index],vertices_inner[index + 1] }.getDirection();
	glm::vec2 ball_dir = ball_path.getDirection();

	float sign = -cross2d(ball_dir, border_dir) < 0.f ? -1.f : 1.f;
	border_dir *= sign;

	float angle = angleBetweenVectors(ball_dir, border_dir);
	glm::vec2 tangent_bounce = glm::normalize(rotate(glm::normalize(border_dir), angle));

	CollisionInfo output;
	output.position = point_collision;
	output.tangentBound = tangent_bounce;

	return output;
}
