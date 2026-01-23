#include "DestroyableObject.h"

#include "Core/CoordinateSystem/CoordinateSystem.h"
#include "Game/Math/AABB/AABB.h"
#include "Game/Math/Arkanoid_Math.h"

void DestroyableObject::SetMesh(const std::vector<glm::vec2>& mesh, const float& radius) {
	this->mesh = mesh;
	this->triangles = MakeTriangulationEarClipping(mesh);
	this->collision_border = GenerateRadiusBorder(this->mesh, radius, isClockwise(this->mesh, true));

	// fix normals
	if (!isClockwise(this->collision_border, true))
		std::reverse(this->collision_border.begin(), this->collision_border.end());


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
	colorAnim = colorTouched;
}

bool DestroyableObject::IsShouldDelete() {
	return time_fly_after_crack >= time_fly_after_crack_valueMAX;
}

void DestroyableObject::Update() {
	glm::vec2 center = (aabb.min + aabb.max) / 2.f;
	float sin_height_offset = std::sinf(engine::time::GetProgrammTime() + center.y) * 0.5f + 0.5f;

	colorAnim = colorAnim + engine::time::GetDeltaTime() * 5.f * ( glm::mix(color, colorShining, sin_height_offset * 0.5f) - colorAnim);

	UpdateCrack();
}

void DestroyableObject::UpdateCrack() {
	if (Health <= 0.f) {
		time_fly_after_crack += engine::time::GetDeltaTime();

		float speedFlyTriangles = 2.f;

		for (size_t i = 0; i < triangles.size(); i++) {
			triangles[i].p1 += direction_move_triangle[i] * engine::time::GetDeltaTime() * speedFlyTriangles;
			triangles[i].p2 += direction_move_triangle[i] * engine::time::GetDeltaTime() * speedFlyTriangles;
			triangles[i].p3 += direction_move_triangle[i] * engine::time::GetDeltaTime() * speedFlyTriangles;
		}
	}
}

void DestroyableObject::Draw(QuadInstanced& renderer, TriangleInstanced& triangles_renderer) {

	/*
	for (size_t i = 0; i < this->mesh.size(); i++) {
		const glm::vec2& begin = this->mesh[i];
		const glm::vec2& end   = this->mesh[(i + 1) % this->mesh.size()];

		renderer.AddLine(begin, end,  2.f, glm::vec4(1.f, 0.f, 0.f, 1.f), TranslateGlobalToScreen);
	}
	*/


	for (size_t i = 0; i < triangles.size(); i++) {
		triangles_renderer.Add(triangles[i], colorAnim, TranslateGlobalToScreen);
	}
}

void DestroyableObject::DrawDebug(QuadInstanced& renderer, const bool& drawNormals) {
	if (this->collision_border.empty()) return;

	for (size_t i = 0; i < this->collision_border.size(); i++) {
		glm::vec2& begin = collision_border[i];
		glm::vec2& end   = collision_border[(i + 1) % collision_border.size()];

		renderer.AddLine(begin, end, 1.f, glm::vec4(0.f, 1.f, 0.f, 1.f), TranslateGlobalToScreen);

		if (drawNormals) {
			glm::vec2 perp = perp_normalized(getDirection(begin, end)) * 0.05f;
			const glm::vec4 color_normal = glm::vec4{ 1.f,0.4f,1.f,1.f };

			renderer.AddLine(begin, begin + perp, 1.f, color_normal, TranslateGlobalToScreen);
			renderer.AddLine(end, end + perp, 1.f, color_normal, TranslateGlobalToScreen);
		}

	}

	if (mesh.empty()) return;

	for (size_t i = 0; i < mesh.size(); i++) {
		glm::vec2& begin = mesh[i];
		glm::vec2& end = mesh[(i + 1) % mesh.size()];

		renderer.AddLine(begin, end, 1.f, glm::vec4(1.f, 1.f, 0.2f, 1.f), TranslateGlobalToScreen);

	}


	renderer.AddRectangleLines(this->aabb.min, this->aabb.max, 1.f, glm::vec4(0.f, 0.f, 1.f, 0.2f), TranslateGlobalToScreen);
}

void DestroyableObject::UpdateColor() {
	float time = 1.f - Health / MaxHealth;
	color = colorFrom + time * (colorTo - colorFrom);
}

void DestroyableObject::SetCollisionPos(const glm::vec2& Collision) {
	for (size_t i = 0; i < direction_move_triangle.size(); i++) {
		direction_move_triangle[i] = glm::normalize(getDirection(Collision, triangles[i].getCenter()));
	}
}

bool DestroyableObject::IsCollidable() {
	return Health > 0.f;
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
		direction_move_triangle.resize(triangles.size());




		Health = 100.f;
		MaxHealth = 100.f;



		colorFrom = glm::vec4(90.f/255.f, 55.f/255.f, 219.f/255.f, 1.f);
		colorTo = glm::vec4(1.f, 0.f, 0.f, 0.f);
		colorTouched = glm::vec4(1.f, 1.f, 1.f, 1.f);
		colorShining = glm::vec4(1.f, 0.5f, 0.f, 1.f);
		colorCracked = glm::vec4(1.f, 0.3f, 0.3f, 0.3f);

		color = colorFrom;
		colorAnim = color;

		UpdateColor();


	}

}