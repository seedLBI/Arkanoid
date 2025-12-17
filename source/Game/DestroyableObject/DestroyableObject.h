#ifndef DESTROYABLE_OBJECT_H
#define DESTROYABLE_OBJECT_H

#include "Core/Engine/Engine.h"

#include "Game/Math/AABB/AABB.h"
#include "Game/Math/Triangulation/EarClipping/Triangulation.EarClipping.h"

#include <glm/glm.hpp>
#include <vector>
#include <nlohmann/json.hpp>

#include "Rendering/RenderObjects/QuadInstanced/Render.Quad.Instanced.h"
#include "Rendering/RenderObjects/TriangleInstanced/Render.Triangle.Instanced.h"




struct DestroyableObject {
	std::vector<glm::vec2> mesh;
	std::vector<glm::vec2> collision_border;
	std::vector<Triangle> triangles;
	AABB_Region aabb;

	float Health	= 100.f;
	float MaxHealth = 100.f;
	glm::vec4 color;
	glm::vec4 colorAnim;

	// for gradient
	glm::vec4 colorFrom;
	glm::vec4 colorTo;
	glm::vec4 colorTouched;
	glm::vec4 colorShining;


	void SetDamage(const float& damage);
	bool IsShouldDelete();

	void Update();

	void UpdateColor();

	void Draw(QuadInstanced& quads_renderer, TriangleInstanced& triangles_renderer);
	void DrawDebug(QuadInstanced& renderer);

	void SetMesh(const std::vector<glm::vec2>& mesh, const float& radius);

	const AABB_Region& GetCurrentAABB();
	const std::vector<glm::vec2>& GetVertices();
	const std::vector<glm::vec2>& GetVertices_OriginalBorder();

	void UpdateRadius(const float& radius);

	nlohmann::json Save();
	void Load(const nlohmann::json& data);


};






#endif