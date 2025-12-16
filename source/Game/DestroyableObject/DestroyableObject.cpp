#include "DestroyableObject.h"

#include "Core/CoordinateSystem/CoordinateSystem.h"
#include "Game/Math/AABB/AABB.h"
#include "Game/Math/Arkanoid_Math.h"

void DestroyableObject::SetMesh(const std::vector<glm::vec2>& mesh, const float& radius) {
	this->mesh = mesh;

	this->collision_border = GenerateRadiusBorder(this->mesh, radius, isClockwise(this->mesh, true));
	this->aabb = GetAABB(this->collision_border);
}

const AABB_Region& DestroyableObject::GetCurrentAABB() {
	return aabb;
}

const std::vector<glm::vec2>& DestroyableObject::GetVertices() {
	return collision_border;
}
const std::vector<glm::vec2>& DestroyableObject::GetVertices_OriginalBorder() {
	return mesh;
}

void DestroyableObject::UpdateRadius(const float& radius) {
	this->collision_border = GenerateRadiusBorder(this->mesh, radius, isClockwise(this->mesh, true));
	this->aabb = GetAABB(this->collision_border);
}


void DestroyableObject::SetDamage(const float& damage) {
	Health = glm::clamp(Health - damage, 0.f, MaxHealth);
	UpdateColor();
}

bool DestroyableObject::IsShouldDelete() {
	return Health <= 0.f;
}


void DestroyableObject::Draw(QuadInstanced& renderer, TriangleInstanced& triangles_renderer) {

	if (this->mesh.empty()) return;

	for (size_t i = 0; i < this->mesh.size(); i++) {
		const glm::vec2& begin = this->mesh[i];
		const glm::vec2& end   = this->mesh[(i + 1) % this->mesh.size()];

		renderer.AddLine(begin, end,  2.f, glm::vec4(1.f, 0.f, 0.f, 1.f), TranslateGlobalToScreen);
	}


	for (size_t i = 0; i < triangles.size(); i++) {
		triangles_renderer.Add(triangles[i], color, TranslateGlobalToScreen);
	}
}

void DestroyableObject::DrawDebug(QuadInstanced& renderer) {
	if (this->collision_border.empty()) return;

	for (size_t i = 0; i < this->collision_border.size(); i++) {
		glm::vec2& begin = collision_border[i];
		glm::vec2& end   = collision_border[(i + 1) % collision_border.size()];

		renderer.AddLine(begin, end, 1.f, glm::vec4(0.f, 1.f, 0.f, 1.f), TranslateGlobalToScreen);

		glm::vec2 perp = perp_normalized(getDirection(begin, end)) * 0.05f;
		const glm::vec4 color_normal = glm::vec4{ 1.f,0.4f,1.f,1.f };

		renderer.AddLine(begin, begin + perp, 1.f, color_normal, TranslateGlobalToScreen);
		renderer.AddLine(end, end + perp, 1.f, color_normal, TranslateGlobalToScreen);
	}

	renderer.AddRectangleLines(this->aabb.min, this->aabb.max, 1.f, glm::vec4(0.f, 0.f, 1.f, 0.2f), TranslateGlobalToScreen);
}

void DestroyableObject::UpdateColor() {
	float time = 1.f - Health / MaxHealth;
	color = colorFrom + time * (colorTo - colorFrom);
}

nlohmann::json DestroyableObject::Save() {
	nlohmann::json data;

	for (size_t i = 0; i < mesh.size(); i++) {
		data["mesh"][i]["x"] = mesh[i].x;
		data["mesh"][i]["y"] = mesh[i].y;
	}

	return data;
}

void DestroyableObject::Load(const nlohmann::json& data) {
	if (data.contains("mesh")) {

		mesh.clear();
		collision_border.clear();

		for (const auto& [key, dataMesh] : data["mesh"].items()) {
			glm::vec2 point_mesh;
			point_mesh.x = dataMesh["x"].get<float>();
			point_mesh.y = dataMesh["y"].get<float>();

			mesh.push_back(point_mesh);
		}
		
		if (mesh.front() == mesh.back())
			mesh.pop_back();

		if (isClockwise(mesh, true) == false) {
			std::reverse(mesh.begin(), mesh.end());
		}

		triangles = MakeTriangulationEarClipping(mesh);

		Health = 100.f;
		MaxHealth = 100.f;

		colorFrom = glm::vec4(1.f, 1.f, 1.f, 1.f);
		colorTo = glm::vec4(1.f, 0.f, 0.f, 0.f);

		UpdateColor();


	}

}