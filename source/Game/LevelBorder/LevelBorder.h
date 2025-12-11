#ifndef LEVEL_BORDER_H
#define LEVEL_BORDER_H

#include <optional>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "Game/CollisionInfo/CollisionInfo.h"

#include "Rendering/RenderObjects/DebugLine/Render.Debug.Line.h"
#include "Rendering/RenderObjects/QuadInstanced/Render.Quad.Instanced.h"	


class LevelBorder {
public:
	LevelBorder();
	LevelBorder(const std::vector<glm::vec2>& vertices, const float& global_radius);
	~LevelBorder();

	void Draw(QuadInstanced& renderer);
	void Draw(DebugLine& renderer);
	void DrawDebug(QuadInstanced& renderer);

	const std::vector<glm::vec2>& GetVertices();
	const std::vector<glm::vec2>& GetVertices_OriginalBorder();



	void SetVertices(const std::vector<glm::vec2>& vertices, const float& global_radius);
	void UpdateRadius(const float& global_radius);

	nlohmann::json Save();
	void Load(const nlohmann::json& data);

private:

	const glm::vec4 color_borders = glm::vec4(1.f);

	std::vector<glm::vec2> vertices;

	std::vector<glm::vec2> vertices_inner;
};



#endif