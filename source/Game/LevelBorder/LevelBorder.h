#ifndef LEVEL_BORDER_H
#define LEVEL_BORDER_H

#include <optional>
#include <glm/glm.hpp>

#include "Game/CollisionInfo/CollisionInfo.h"

#include "Rendering/RenderObjects/DebugLine/Render.Debug.Line.h"


class LevelBorder {
public:
	LevelBorder(const std::vector<glm::vec2>& vertices);
	~LevelBorder();

	void Draw(DebugLine& renderer);

	const std::vector<glm::vec2>& GetVertices();

	std::optional<CollisionInfo> GetCollision(const glm::vec2& begin, const glm::vec2& end);

private:

	const float MaxSampleRateCircle = 120;

	const glm::vec4 color_borders = glm::vec4(1.f);

	std::vector<glm::vec2> vertices;

	std::vector<glm::vec2> vertices_outer;
	std::vector<glm::vec2> vertices_inner;

	std::vector<glm::vec2> GenerateRadiusBorder(const float& radius, const bool& outer_border);
};



#endif