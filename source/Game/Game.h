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

	bool debug_flag_enable_path_tracer = false;

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

	float speedAnim = 1.f;
	float speedAnimValue = 1.f;

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

	std::optional<CollisionInfo> TryCollision(Ball& ball_, const AABB_Region& aabb, const std::vector<glm::vec2>& vertices, bool alwaysInside = false);
	std::optional<CollisionInfo> TryCollision(Ball& ball_, const std::vector<glm::vec2>& vertices, bool alwaysInside = false);

	enum COLLISION_OBJECT_TYPE {
		COLLISION_NOTHING,
		COLLISION_DESTROYABLE,
		COLLISION_BORDER,
		COLLISION_PLAYER
	};

	struct ClosestCollisionData {
		size_t index = 0;
		COLLISION_OBJECT_TYPE object = COLLISION_DESTROYABLE;
		std::optional<CollisionInfo> info;
	};

	ClosestCollisionData GetClosestCollision(
		Ball& ball_, 
		const std::vector<std::optional<CollisionInfo>>& collision_destroyable,
		const std::optional<CollisionInfo>& collision_player,
		const std::optional<CollisionInfo>& collision_border
	);



	bool ResolveCollision(Ball& ball_);
};

#endif