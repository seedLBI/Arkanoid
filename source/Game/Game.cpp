#include "Game.h"

#include "Game/Math/Arkanoid_Math.h"
#include "Core/CoordinateSystem/CoordinateSystem.h"

Game::Game() {

}

Game::~Game() {

}

void Game::Draw(QuadInstanced& quads_renderer, DebugCircle& circles_renderer) {

	if (path.size() > 1) {
		for (size_t i = 0; i < path.size() - 1; i++) {
			quads_renderer.AddLine(path[i], path[i + 1], 1.f, glm::vec4(0.f, 1.f, 0.f, 1.f), TranslateGlobalToScreen);


			circles_renderer.Add(path[i], 2.f, glm::vec4(1.f, 0.f, 0.f, 1.f), TranslateGlobalToScreen);

		}
	}

	player.Draw(quads_renderer);
	player.DrawDebug(quads_renderer);

	border.Draw(quads_renderer);
	border.DrawDebug(quads_renderer);

	for (auto& obj: objs)
		obj.Draw(quads_renderer);

	ball.Draw(circles_renderer);



}

void Game::Update() {
	
	player.Update();

	if (engine::time::GetProgrammTime() <= 1.f)
		return;


	if (engine::input::IsKeyPressed(KEY_Z)) {
		ball.tangent = glm::normalize(ballSpawn.tangent);
		ball.path.begin = ballSpawn.globalPos;
		ball.path.end = ballSpawn.globalPos + glm::normalize(ballSpawn.tangent) * ball.radius;
		path.clear();
	}


	path.push_back(ball.path.begin);

	float deltaTime = engine::time::GetDeltaTime();
	float speed = ball.speed;
	glm::vec2& tangent = ball.tangent;

	glm::vec2 nextDirection = tangent * speed * deltaTime;
	ball.path.begin = ball.path.end;
	ball.path.end   = ball.path.begin + nextDirection;



	//check collisions
	int count_check_collisions = 0;
	const int MAX_COUNT_COLLISIONS_CHECK = 1;

	while (true) {

		bool have_collision = false;

		if (ResolveCollision(border.GetVertices())) {
			have_collision = true;
		}
		else if (ResolveCollision(player.GetVertices())) {
			have_collision = true;
		}
		else {
			for (size_t i = 0; i < objs.size(); i++) {
				if (ResolveCollision(objs[i].GetCurrentAABB(), objs[i].GetVertices())) {
					objs.erase(objs.begin() + i);
					have_collision = true;
					break;
				}

			}
		}
		
		if (have_collision) {
			count_check_collisions++;
		}
		else
			break;

		if (count_check_collisions >= MAX_COUNT_COLLISIONS_CHECK)
			break;

	}
	
}



bool Game::ResolveCollision(const AABB_Region& aabb, const std::vector<glm::vec2>& vertices) {

	bool collision_have = Collision_Segment_and_AABB(ball.path, ball.radius, aabb);
	if (!collision_have) return false;

	return ResolveCollision(vertices);

}
bool Game::ResolveCollision(const std::vector<glm::vec2>& vertices) {

	int count_recals = 0;
	int max_recalc = 3;

	while (true) {
		std::optional<CollisionInfo> collision_point;
		if (count_recals == 0)
			collision_point = GetCollision(vertices, ball.path.begin, ball.path.end);
		else
			collision_point = GetCollision(vertices, ball.path.begin + ball.tangent * 0.03f, ball.path.end);

		if (collision_point.has_value()) {

			const CollisionInfo& collision = collision_point.value();

			float length_after_collision = glm::length(ball.path.end - collision.position);

			ball.tangent = collision.tangentBound;
			ball.path.begin = collision.position;
			ball.path.end = collision.position + collision.tangentBound * length_after_collision;


			path.push_back(ball.path.begin);

		}
		else {
			break;
		}

		count_recals++;
		if (count_recals >= max_recalc)
			break;
	}

	return count_recals > 0;
}


void Game::Load(const nlohmann::json& dataLevel) {
	this->data = dataLevel;

	if (data.contains("ballSpawn")) {
		ballSpawn.Load(data["ballSpawn"]);

		ball.radius = ballSpawn.global_radius;
		ball.speed = 4.7f;
		ball.tangent = glm::normalize(ballSpawn.tangent);
		ball.path.begin = ballSpawn.globalPos;
		ball.path.end = ballSpawn.globalPos + glm::normalize(ballSpawn.tangent) * ball.radius;
	}

	if (data.contains("border")) {
		border.Load(data["border"]);
		border.UpdateRadius(ballSpawn.global_radius);
	}

	if (data.contains("player")) {
		player.Load(data["player"]);
		player.UpdateRadius(ballSpawn.global_radius);
	}

	if (data.contains("destroyable")) {
		objs.clear();
		for (const auto& [key, destroyableData] : data["destroyable"].items()) {
			DestroyableObject temp;
			temp.Load(destroyableData);
			temp.UpdateRadius(ballSpawn.global_radius);
			objs.emplace_back(temp);
		}
	}

}
