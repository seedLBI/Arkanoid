#ifndef DESTROYABLE_OBJECT_H
#define DESTROYABLE_OBJECT_H

#include "Game/Math/AABB/AABB.h"
#include <glm/glm.hpp>
#include <vector>
#include <nlohmann/json.hpp>

#include "Rendering/RenderObjects/QuadInstanced/Render.Quad.Instanced.h"

struct DestroyableObject {
	std::vector<glm::vec2> mesh;
	std::vector<glm::vec2> collision_border;
	AABB_Region aabb;


	void Draw(QuadInstanced& renderer);
	void DrawDebug(QuadInstanced& renderer);

	void SetMesh(const std::vector<glm::vec2>& mesh, const float& radius);


	void UpdateRadius(const float& radius);

	nlohmann::json Save();
	void Load(const nlohmann::json& data);


};






#endif