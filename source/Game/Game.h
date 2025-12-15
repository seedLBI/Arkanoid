#ifndef GAME_H
#define GAME_H

#include <vector>
#include <deque>
#include <nlohmann/json.hpp>


#include "Core/Engine/Engine.h"


#include "Game/BallSpawnPosition/BallSpawnPosition.h"
#include "Game/DestroyableObject/DestroyableObject.h"
#include "Game/CollisionInfo/CollisionInfo.h"
#include "Game/LevelBorder/LevelBorder.h"
#include "Game/Player/Player.h"
#include "Game/Ball/Ball.h"

#include "Game/Math/Triangle/Triangle.h"
#include "Game/Math/Triangulation/Greedy/Triangulation.Greedy.h"
#include "Game/Math/Triangulation/EarClipping/Triangulation.EarClipping.h"

#include "Rendering/RenderObjects/TriangleInstanced/Render.Triangle.Instanced.h"
#include "Rendering/RenderObjects/QuadInstanced/Render.Quad.Instanced.h"
#include "Rendering/RenderObjects/DebugCircle/Render.Debug.Circle.h"



class Game {
private:

	glm::vec2 debug_ball_begin = {0.f,0.f};
	glm::vec2 debug_ball_end = {0.f,-0.01f};
	Ball debug_ball;
	float debug_FPS = 1.f;
	float debug_deltaTime = 0.16f;

	float debug_speed = 3.f;

	float debug_length_path = 1.f;


	std::vector<glm::vec2> debug_path;
	float GetLengthPath();


	void UpdateDebugInput();


	void SetNextPosition(Ball& ball_, const  glm::vec2 tangent, const  glm::vec2 begin, const glm::vec2 end);

public:
	Game();
	~Game();

	void Draw(TriangleInstanced& triangles_renderer, QuadInstanced& quads_renderer, DebugCircle& circles_renderer);
	void Update();

	void DrawDebug(TriangleInstanced& triangles_renderer, QuadInstanced& quads_renderer, DebugCircle& circles_renderer);
	void UpdateDebug();



	void Load(const nlohmann::json& dataLevel);

private:
	Player player;
	Ball ball;

	float speedAnim = 0.f;
	float speedAnimValue = 0.2f;

	BallSpawnPosition ballSpawn;
	LevelBorder border;
	std::vector<DestroyableObject> objs;


	struct TrailPoint {
		glm::vec2 pos;
		float timeCreation;
	};
	std::deque<TrailPoint> trail;
	float LifeTimePoints = 0.5f;
	float LengthTrail = 0.5f;
	int MAX_count_trail = 130;


	nlohmann::json data;

	void TrailUpdate();
	void UpdateAnimValues();

	void RespawnBall();
	
	bool ResolveCollision(const AABB_Region& aabb, const std::vector<glm::vec2>& vertices, Ball& ball_);
	bool ResolveCollision(const std::vector<glm::vec2>& vertices, Ball& ball_);
};

#endif