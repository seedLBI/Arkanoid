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

	for (size_t i = 0; i < vertices_inner.size(); i++)
		renderer.AddLine(
			vertices_inner[i], 
			vertices_inner[(i + 1) % vertices_inner.size()], 
			1.f, 
			glm::vec4(1.0f, 0.6f, 0.3f, 1.f), 
			TranslateGlobalToScreen);
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

		std::cout << "(" << x << ", " << y << "), ";

	}

	if (vertices.front() == vertices.back())
		vertices.pop_back();

	if (isClockwise(vertices, true) == false) {
		std::reverse(vertices.begin(), vertices.end());
	}


	vertices_inner = GenerateRadiusBorder(this->vertices, 0.05f, !isClockwise(this->vertices, true));

}