#include "Arkanoid_Math.h"
#include <glm/gtc/constants.hpp>
#include <iostream>


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


bool isIntersectPointCircle(const glm::vec2& point, const glm::vec2& posCircle, const float& radius) {
	return glm::distance(point, posCircle) < radius;
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

bool isClockwise(const std::vector<glm::vec2>& polygon, bool yAxisUp ) {
	if (polygon.size() < 3) return false;

	float sum = 0.0f;
	size_t n = polygon.size();
	size_t vertexCount = polygon[0] == polygon[n - 1] ? n - 1 : n;

	for (size_t i = 0; i < vertexCount; i++) {
		size_t j = (i + 1) % vertexCount;
		sum += (polygon[j].x - polygon[i].x) *
			(polygon[j].y + polygon[i].y);
	}

	if (yAxisUp) {
		return sum > 0.0f;
	}
	else {
		return sum < 0.0f;
	}
}


std::optional<CollisionInfo> GetCollision(const std::vector<glm::vec2>& border_vertices, const glm::vec2& begin, const glm::vec2& end) {
	const Segment ball_path{ begin,end };

	std::vector<std::pair<size_t, glm::vec2>> collisions;


	for (size_t i = 0; i < border_vertices.size() - 1; i++) {
		Segment border{ border_vertices[i], border_vertices[i + 1] };

		float time = getTimeCollisionBetweenTwoSegment(border, ball_path);
		float time2 = getTimeCollisionBetweenTwoSegment(ball_path, border);

		if (time > 1.f || time < 0.f || time2 > 1.f || time2 < 0.f)
			continue;

		glm::vec2 point = lerp(border, time);
		collisions.push_back({ i, point });
	}

	if (collisions.empty())
		return std::nullopt;

	std::sort(collisions.begin(), collisions.end(), [&begin](const auto& a, const auto& b) {
		return glm::length(a.second - begin) > glm::length(b.second - begin);
		});

	const auto& nereast_collision = collisions.back();

	const glm::vec2& point_collision = nereast_collision.second;
	const size_t& index = nereast_collision.first;

	glm::vec2 border_dir = Segment{ border_vertices[index],border_vertices[index + 1] }.getDirection();
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


std::vector<glm::vec2> GenerateRadiusBorder(const std::vector<glm::vec2>& original_border, const float& radius, const bool& outer_border) {

	if (original_border.size() == 1)
		return {};

	std::vector<glm::vec2> output;

	const float MaxSampleRateCircle = 120;

	const std::vector<glm::vec2>& vertices = original_border;

	for (size_t i = 1; i < vertices.size(); i++) {

		Segment A, B;

		if (i == vertices.size() - 1) {
			A = { vertices[i - 1], vertices[i] };
			B = { vertices[i], vertices[1] };
		}
		else {
			A = { vertices[i - 1], vertices[i] };
			B = { vertices[i], vertices[i + 1] };
		}

		glm::vec2 dirA = A.begin - A.end;
		glm::vec2 dirB = B.begin - B.end;

		glm::vec2 perpA = perp_normalized(dirA) * radius;
		glm::vec2 perpB = perp_normalized(dirB) * radius;

		float sign_radius = outer_border == true ? -1.f : 1.f;

		perpA *= sign_radius;
		perpB *= sign_radius;

		Segment A_radius{ A.begin + perpA, A.end + perpA };
		Segment B_radius{ B.begin + perpB, B.end + perpB };

		glm::vec2 dirA_radius = A_radius.getDirection();
		glm::vec2 dirB_radius = B_radius.getDirection();


		float cross_dir_radius = cross2d(dirA_radius, dirB_radius) * (-1.f) * sign_radius;

		bool circle_moment = cross_dir_radius > 0 ? false : true;

		if (circle_moment) {
			float sign = cross2d(dirA, dirB) > 0.f ? 1.f : 3.f;

			const float& PI = glm::pi<float>();

			float alphaA = atan2(dirA);
			float alphaB = atan2(dirB);

			float angleA = alphaA + PI / 2.f * sign;
			float angleB = alphaB + PI / 2.f * sign;

			float diff = angleB - angleA;

			if (abs(diff) <= 0.03f) {
				output.push_back(A_radius.end);

				continue;
			}


			diff = atan2({ cosf(diff), sinf(diff) });

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