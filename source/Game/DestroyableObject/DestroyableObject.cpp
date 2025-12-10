#include "DestroyableObject.h"

#include "Core/CoordinateSystem/CoordinateSystem.h"
#include "Game/Math/AABB/AABB.h"
#include "Game/Math/Arkanoid_Math.h"

void DestroyableObject::SetMesh(const std::vector<glm::vec2>& mesh, const float& radius) {
	this->mesh = mesh;
	this->mesh.push_back(this->mesh.front());


	this->collision_border = GenerateRadiusBorder(this->mesh, radius, isClockwise(this->mesh, true));



	this->aabb = GetAABB(this->collision_border);
}

const AABB_Region& DestroyableObject::GetCurrentAABB() {
	return aabb;
}

const std::vector<glm::vec2>& DestroyableObject::GetVertices() {
	return mesh;
}
const std::vector<glm::vec2>& DestroyableObject::GetVertices_OriginalBorder() {
	return collision_border;
}

void DestroyableObject::UpdateRadius(const float& radius) {
	this->collision_border = GenerateRadiusBorder(this->mesh, radius, isClockwise(this->mesh, true));
	this->aabb = GetAABB(this->collision_border);
}



void DestroyableObject::Draw(QuadInstanced& renderer) {

	if (this->mesh.empty()) return;

	for (size_t i = 0; i < this->mesh.size() - 1; i++) {
		const glm::vec2& begin = this->mesh[i];
		const glm::vec2& end   = this->mesh[i + 1];

		renderer.AddLine(begin, end, glm::vec4(1.f, 0.f, 0.f, 1.f), TranslateGlobalToScreen);
	}


}

void DestroyableObject::DrawDebug(QuadInstanced& renderer) {
	if (this->collision_border.empty()) return;
	for (size_t i = 0; i < this->collision_border.size() - 1; i++) {
		const glm::vec2& begin = this->collision_border[i];
		const glm::vec2& end = this->collision_border[i + 1];

		renderer.AddLine(begin, end, glm::vec4(0.f, 1.f, 0.f, 1.f), TranslateGlobalToScreen);
	}

	renderer.AddRectangleLines(this->aabb.min, this->aabb.max, 2.f, glm::vec4(0.f, 0.f, 1.f, 1.f), TranslateGlobalToScreen);
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

	}

}