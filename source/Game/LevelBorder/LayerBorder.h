#ifndef LAYER_BORDER_H
#define LAYER_BORDER_H

#include <optional>
#include <glm/glm.hpp>

#include "Game/CollisionInfo/CollisionInfo.h"

#include "Rendering/RenderObjects/DebugLine/Render.Debug.Line.h"


class LayerBorder {
public:
	LayerBorder(const std::vector<glm::vec2>& vertices);
	~LayerBorder();

	void Draw(DebugLine& renderer);

	std::optional<CollisionInfo> GetCollision(const glm::vec2& begin, const glm::vec2& end);

private:
	const glm::vec4 color_borders = glm::vec4(1.f);

	std::vector<glm::vec2> vertices;
};



#endif