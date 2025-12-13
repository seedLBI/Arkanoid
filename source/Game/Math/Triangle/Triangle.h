#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Rendering/RenderObjects/QuadInstanced/Render.Quad.Instanced.h"
#include <glm/glm.hpp>

struct Triangle {
	glm::vec2 p1, p2, p3;

	void DrawLine(QuadInstanced& renderer);
};

#endif
