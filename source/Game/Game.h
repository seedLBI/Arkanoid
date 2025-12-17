#ifndef GAME_H
#define GAME_H

#include <vector>
#include <deque>
#include <nlohmann/json.hpp>


#include "Core/Engine/Engine.h"


#include "Game/BallSpawnPosition/BallSpawnPosition.h"
#include "Game/DestroyableObject/DestroyableObject.h"
#include "Game/CollisionInfo/CollisionInfo.h"
#include "Game/Border/Border.h"
#include "Game/Player/Player.h"
#include "Game/Ball/Ball.h"

#include "Game/Math/Triangle/Triangle.h"
#include "Game/Math/Triangulation/Greedy/Triangulation.Greedy.h"
#include "Game/Math/Triangulation/EarClipping/Triangulation.EarClipping.h"

#include "Rendering/RenderObjects/TriangleInstanced/Render.Triangle.Instanced.h"
#include "Rendering/RenderObjects/QuadInstanced/Render.Quad.Instanced.h"
#include "Rendering/RenderObjects/DebugCircle/Render.Debug.Circle.h"
#include "Rendering/RenderObjects/Quad/Render.Quad.h"



class Game {
private:

	glm::vec2 debug_ball_begin = {0.f,0.f};
	glm::vec2 debug_ball_end = {0.f,-0.01f};
	Ball  debug_ball;
	float debug_FPS = 1.f;
	float debug_deltaTime = 0.16f;
	float debug_speed = 3.f;
	float debug_length_path = 1.f;
	bool  debug_flag_enable_path_tracer = false;

	std::vector<glm::vec2> debug_path;
	float GetLengthPath();
	void UpdateDebugInput();
public:
	Game();
	~Game();

	void Draw(TriangleInstanced& triangles_renderer, QuadInstanced& quads_renderer, DebugCircle& circles_renderer);
	void Update();

	void Load(const nlohmann::json& dataLevel);



	void DrawDebug(TriangleInstanced& triangles_renderer, QuadInstanced& quads_renderer, DebugCircle& circles_renderer);
	void UpdateDebug();


private:
	Player player;
	Ball ball;

	float speedAnim = 1.f;
	float speedAnimValue = 1.f;

	BallSpawnPosition ballSpawn;
	Border border;
	std::vector<DestroyableObject> objs;
	
	nlohmann::json data;

	Shader* shader_background;
	Quad quad_fullscreen;


	void UpdateAnimValues();

	void RespawnBall();

	std::optional<CollisionInfo> TryCollision(
		Ball& ball_, 
		const AABB_Region& aabb, 
		const std::vector<glm::vec2>& vertices, 
		COLLISION_PUSH_TYPE collision_push_type = ALWAYS_PUSH_INSIDE);

	std::optional<CollisionInfo> TryCollision(
		Ball& ball_, 
		const std::vector<glm::vec2>& vertices, 
		COLLISION_PUSH_TYPE collision_push_type = ALWAYS_PUSH_INSIDE);


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

	void SetNextPosition(Ball& ball_, const  glm::vec2 tangent, const  glm::vec2 begin, const glm::vec2 end);

	void ReactToCollision(const ClosestCollisionData& data);
	void ApplyCollision(Ball& ball_, const ClosestCollisionData& data);

	bool ResolveCollision(Ball& ball_);
};

#endif