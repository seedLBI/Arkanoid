#include "Game.h"

#include "Game/Math/Arkanoid_Math.h"

Game::Game() {

}

Game::~Game() {

}

void Game::Draw(QuadInstanced& quads_renderer, DebugCircle& circles_renderer) {
	player.Draw(quads_renderer);
	border.Draw(quads_renderer);

	for (auto& obj: objs)
		obj.Draw(quads_renderer);

	ball.Draw(circles_renderer);

}

void Game::Update() {
	player.Update();

	if (engine::time::GetProgrammTime() <= 1.f)
		return;

	float deltaTime = engine::time::GetDeltaTime();
	float speed = ball.speed;
	glm::vec2& tangent = ball.tangent;

	glm::vec2 nextDirection = tangent * speed * deltaTime;
	//ball.path.begin +=  nextDirection;

	ball.path.begin = ball.path.end;
	ball.path.end = ball.path.begin + nextDirection;



	//check collisions
	if (ResolveCollision(player.GetCurrentAABB(), player.GetVertices())) {

	}
	else if (ResolveCollision(border.GetVertices())) {

	}
	else {
		for (size_t i = 0; i < objs.size(); i++) {
			if (ResolveCollision(objs[i].aabb, objs[i].collision_border)) {
				objs.erase(objs.begin() + i);
				break;
			}

		}
	}


}

bool Game::ResolveCollision(const std::vector<glm::vec2>& vertices) {

	auto collision_point = GetCollision(vertices, ball.path.begin, ball.path.end);

	if (collision_point.has_value()) {
		const CollisionInfo& collision = collision_point.value();

		float length_after_collision = glm::length(ball.path.end - collision.position);
		ball.tangent = collision.tangentBound;
		ball.path.begin = collision.position;
		ball.path.end = collision.position + collision.tangentBound * length_after_collision;
		return true;
	}

	return false;
}
bool Game::ResolveCollision(const AABB_Region& aabb, const std::vector<glm::vec2>& vertices) {
	bool collision_have = Collision_Segment_and_AABB(ball.path, ball.radius, aabb);
	if (collision_have) {

		auto collision_point = GetCollision(vertices, ball.path.begin, ball.path.end);

		if (collision_point.has_value()) {
			const CollisionInfo& collision = collision_point.value();

			float length_after_collision = glm::length(ball.path.end - collision.position);
			ball.tangent = collision.tangentBound;
			ball.path.begin = collision.position;
			ball.path.end = collision.position + collision.tangentBound * length_after_collision;

			return true;
		}
	}
	return false;
}


void Game::Load(const nlohmann::json& dataLevel) {
	this->data = dataLevel;

	if (data.contains("ballSpawn")) {
		ballSpawn.Load(data["ballSpawn"]);

		ball.radius = ballSpawn.global_radius;
		ball.speed = 1.2f;
		ball.tangent = glm::normalize(ballSpawn.tangent);
		ball.path.begin = ballSpawn.globalPos;
		ball.path.end = ballSpawn.globalPos + glm::normalize(ballSpawn.tangent) * ball.radius;
	}

	if (data.contains("border")) {
		border.Load(data["border"]);
		border.SetRadius(ballSpawn.global_radius);
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
