#include "LevelBorder.h"

#include "Game/Math/Segment/Segment.h"
#include "Game/Math/Arkanoid_Math.h"
#include "Core/CoordinateSystem/CoordinateSystem.h"

#include "Core/Engine/Engine.h"

#include <algorithm>


LevelBorder::LevelBorder() {

}

LevelBorder::LevelBorder(const std::vector<glm::vec2>& vertices_input, const float& global_radius) {
	vertices = vertices_input;
	vertices_inner = GenerateRadiusBorder(vertices, global_radius, !isClockwise(this->vertices, true));
}

LevelBorder::~LevelBorder() {

}

const std::vector<glm::vec2>& LevelBorder::GetVertices() {
	return vertices_inner;
}

const std::vector<glm::vec2>& LevelBorder::GetVertices_OriginalBorder() {
	return vertices;
}

void LevelBorder::UpdateRadius(const float& global_radius) {
	vertices_inner = GenerateRadiusBorder(vertices, global_radius, !isClockwise(this->vertices, true));
	std::reverse(vertices_inner.begin(), vertices_inner.end());
}

void LevelBorder::Draw(QuadInstanced& renderer) {
	if (vertices.empty())
		return;

	for (size_t i = 0; i < vertices.size(); i++)
		renderer.AddLine(
			vertices[i], 
			vertices[(i + 1) % vertices.size()], 
			2.f, 
			color_borders, 
			TranslateGlobalToScreen);
}

void LevelBorder::DrawDebug(QuadInstanced& renderer) {
	if (vertices.empty())
		return;

	for (size_t i = 0; i < vertices_inner.size(); i++) {

		glm::vec2& begin = vertices_inner[i];
		glm::vec2& end = vertices_inner[(i + 1) % vertices_inner.size()];

		renderer.AddLine(begin,end,1.f,glm::vec4(1.0f, 0.6f, 0.3f, 1.f),TranslateGlobalToScreen);

		glm::vec2 perp = perp_normalized(getDirection(begin, end));
		const glm::vec4 color_normal = glm::vec4{ 1.f,0.f,1.f,1.f };

		renderer.AddLine(begin, begin + perp * 0.05f, 1.f, color_normal, TranslateGlobalToScreen);
		renderer.AddLine(end,   end   + perp * 0.05f, 1.f, color_normal, TranslateGlobalToScreen);


		glm::vec2 left_arrow  = end - rotate( perp, glm::pi<float>() / 3.f) * 0.03f;
		glm::vec2 right_arrow = end - rotate(-perp, -glm::pi<float>() / 3.f) * 0.03f;


		renderer.AddLine(end, left_arrow, 1.f, glm::vec4(0.f, 1.f, 0.f, 1.f), TranslateGlobalToScreen);
		renderer.AddLine(end, right_arrow, 1.f, glm::vec4(0.f, 1.f, 0.f, 1.f), TranslateGlobalToScreen);
		renderer.AddLine(left_arrow, right_arrow, 1.f, glm::vec4(0.f, 1.f, 0.f, 1.f), TranslateGlobalToScreen);



	}
}

void LevelBorder::Draw(DebugLine& renderer) {
	if (vertices.empty())
		return;

	for (size_t i = 0; i < vertices.size(); i++)
		renderer.Add(vertices[i], vertices[(i + 1) % vertices.size()], 3.f, color_borders, TranslateGlobalToScreen);

	for (size_t i = 0; i < vertices_inner.size(); i++)
		renderer.Add(vertices_inner[i], vertices_inner[(i + 1) % vertices_inner.size()], 8.f, glm::vec4(0.2f, 1.f, 0.2f, 1.f), TranslateGlobalToScreen);
}

void LevelBorder::SetVertices(const std::vector<glm::vec2>& vertices, const float& global_radius) {
	this->vertices = vertices;
	vertices_inner = GenerateRadiusBorder(this->vertices, global_radius, !isClockwise(this->vertices, true));
	std::reverse(vertices_inner.begin(), vertices_inner.end());
}

nlohmann::json LevelBorder::Save() {
	nlohmann::json output;
	for (size_t i = 0; i < vertices.size(); i++) {
		output[i]["x"] = vertices[i].x;
		output[i]["y"] = vertices[i].y;
	}
	return output;
}

void LevelBorder::Load(const nlohmann::json& data) {

	vertices.clear();
	vertices.reserve(data.size());

	for (const auto& [key, pointData] : data.items()) {
		
		float x = pointData["x"].get<float>();
		float y = pointData["y"].get<float>();

		vertices.push_back(glm::vec2{ x,y });

	}

	if (vertices.front() == vertices.back())
		vertices.pop_back();

	if (isClockwise(vertices, true) == false) {
		std::reverse(vertices.begin(), vertices.end());
	}

	vertices_inner = GenerateRadiusBorder(this->vertices, 0.05f, !isClockwise(this->vertices, true));
	std::reverse(vertices_inner.begin(), vertices_inner.end());


	printf("\n\nBorder\n[");
	for (size_t i = 0; i < vertices.size(); i++) {

		printf("(%f, %f)", vertices[i].x, vertices[i].y);
		if (i != vertices.size() - 1)
			printf(",");
	}
	printf("]\n");


	printf("\n\nBorderBorder\n[");
	for (size_t i = 0; i < vertices_inner.size(); i++) {

		printf("(%f, %f)", vertices_inner[i].x, vertices_inner[i].y);
		if (i != vertices_inner.size() - 1)
			printf(",");
	}
	printf("]\n");

}