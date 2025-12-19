#ifndef BALL_H
#define BALL_H

#include <vector>
#include <deque>

#include "Core/Engine/Engine.h"

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


	void Update();

	void ResetAll();

private:
	struct TrailPoint {
		glm::vec2 pos;
		float timeCreation;
	};
	std::deque<TrailPoint> trail;
	float LifeTimePoints = 10.5f;
	float LengthTrail = 0.5f;
	int MAX_count_trail = 150;

	void TrailUpdate();


};



#endif // !BALL_H

