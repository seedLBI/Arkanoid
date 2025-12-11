#ifndef GAME_H
#define GAME_H

#include <vector>
#include <nlohmann/json.hpp>


#include "Core/Engine/Engine.h"


#include "Game/BallSpawnPosition/BallSpawnPosition.h"
#include "Game/DestroyableObject/DestroyableObject.h"
#include "Game/CollisionInfo/CollisionInfo.h"
#include "Game/LevelBorder/LevelBorder.h"
#include "Game/Player/Player.h"
#include "Game/Ball/Ball.h"


#include "Rendering/RenderObjects/QuadInstanced/Render.Quad.Instanced.h"
#include "Rendering/RenderObjects/DebugCircle/Render.Debug.Circle.h"

class Game {
public:
	Game();
	~Game();

	void Draw(QuadInstanced& quads_renderer, DebugCircle& circles_renderer);
	void Update();

	void Load(const nlohmann::json& dataLevel);

private:
	Player player;
	Ball ball;
	BallSpawnPosition ballSpawn;
	LevelBorder border;
	std::vector<DestroyableObject> objs;

	std::vector<glm::vec2> path;

	nlohmann::json data;
	
	bool ResolveCollision(const AABB_Region& aabb, const std::vector<glm::vec2>& vertices);
	bool ResolveCollision(const std::vector<glm::vec2>& vertices);
};

#endif