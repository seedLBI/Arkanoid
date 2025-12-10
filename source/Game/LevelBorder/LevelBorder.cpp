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

	vertices.push_back(vertices.front());

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

void LevelBorder::SetRadius(const float& global_radius) {
	vertices_inner = GenerateRadiusBorder(vertices, global_radius, !isClockwise(this->vertices, true));
}

void LevelBorder::Draw(QuadInstanced& renderer) {
	if (vertices.empty())
		return;

	for (size_t i = 0; i < vertices.size() - 1; i++)
		renderer.AddLine(vertices[i], vertices[i + 1], 2.f, color_borders, TranslateGlobalToScreen);
}

void LevelBorder::DrawDebug(QuadInstanced& renderer) {
	if (vertices.empty())
		return;

	for (size_t i = 0; i < vertices_inner.size() - 1; i++)
		renderer.AddLine(vertices_inner[i], vertices_inner[i + 1], 4.f, glm::vec4(0.2f, 1.f, 0.2f, 1.f), TranslateGlobalToScreen);
}

void LevelBorder::Draw(DebugLine& renderer) {
	if (vertices.empty())
		return;

	for (size_t i = 0; i < vertices.size() - 1; i++)
		renderer.Add(vertices[i], vertices[i + 1], 3.f, color_borders, TranslateGlobalToScreen);

	for (size_t i = 0; i < vertices_inner.size() - 1; i++)
		renderer.Add(vertices_inner[i], vertices_inner[i + 1], 8.f, glm::vec4(0.2f, 1.f, 0.2f, 1.f), TranslateGlobalToScreen);
}

void LevelBorder::SetVertices(const std::vector<glm::vec2>& vertices, const float& global_radius) {
	this->vertices = vertices;
	this->vertices.push_back(this->vertices.front());


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
	}

	vertices_inner = GenerateRadiusBorder(this->vertices, 0.05f, !isClockwise(this->vertices, true));

}