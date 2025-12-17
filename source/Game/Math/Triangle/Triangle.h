#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Rendering/RenderObjects/QuadInstanced/Render.Quad.Instanced.h"
#include <glm/glm.hpp>

#include "Game/Math/Arkanoid_Math.h"

struct Triangle {
	glm::vec2 p1, p2, p3;

	void DrawLine(QuadInstanced& renderer);

	float getArea() const;
	glm::vec2 getCenter();
};



bool Collision_Point_and_Triangle(const glm::vec2& point, const Triangle& triangle);


#endif
