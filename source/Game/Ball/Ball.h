#ifndef BALL_H
#define BALL_H

#include <glm/glm.hpp>
#include "Game/Math/Segment/Segment.h"

#include "Rendering/RenderObjects/DebugCircle/Render.Debug.Circle.h"
#include "Rendering/RenderObjects/QuadInstanced/Render.Quad.Instanced.h"


struct Ball {
	glm::vec4 color = glm::vec4(1.f);
	glm::vec2 tangent;
	Segment path = { {0.f,0.f},{0.f,0.0000001f} };
	float radius = 0.05f;
	float speed = 0.1f;

	void Draw(DebugCircle& renderer);
	void DrawDebug(DebugCircle& renderer, QuadInstanced& quads);
};



#endif // !BALL_H

