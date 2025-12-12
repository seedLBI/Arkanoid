#include "Game.h"

#include "Game/Math/Arkanoid_Math.h"
#include "Core/CoordinateSystem/CoordinateSystem.h"

Game::Game() {

}

Game::~Game() {

}

void Game::Draw(QuadInstanced& quads_renderer, DebugCircle& circles_renderer) {

#ifdef _DEBUG
	if (path.size() > 1) {
		for (size_t i = 0; i < path.size() - 1; i++) {
			quads_renderer.AddLine(path[i], path[i + 1], 2.f, glm::vec4(0.f, 1.f, 0.f, 0.2f), TranslateGlobalToScreen);

		}
	}
	player.DrawDebug(quads_renderer);
	border.DrawDebug(quads_renderer);
	for (auto& obj : objs) {
		obj.DrawDebug(quads_renderer);
	}
#endif


	player.Draw(quads_renderer);
	border.Draw(quads_renderer);

	for (auto& obj : objs) {
		obj.Draw(quads_renderer);
	}

	for (size_t i = 0; i < trail.size(); i++) {

		float time = (float)i / (float)MAX_count_trail;
		circles_renderer.Add(trail[i].pos, TranslateScalar_GlobalToScreen(ball.radius) , ball.color * glm::vec4(1.f, 1.f, 1.f, time), TranslateGlobalToScreen);
	}


	ball.Draw(circles_renderer);


}

void Game::RespawnBall() {
	ball.tangent = glm::normalize(ballSpawn.tangent);
	ball.path.begin = ballSpawn.globalPos;
	ball.path.end = ballSpawn.globalPos + glm::normalize(ballSpawn.tangent) * ball.radius;
	trail.clear();
}

void Game::Update() {




	speedAnim = speedAnim + engine::time::GetDeltaTime() * (0.f - speedAnim);
	ball.color = ball.color + engine::time::GetDeltaTime() * 2.f * (glm::vec4(1.f,1.f,1.f,1.f) - ball.color);

	ball.speed = 1.f + speedAnim;

	player.Update();


	MAX_count_trail = (1.f / engine::time::GetDeltaTime()) * 0.5f;
	
	trail.push_back({ ball.path.begin,engine::time::GetProgrammTime()});

	if (trail.size() > MAX_count_trail) {
		trail.erase(trail.begin(), trail.begin() + (trail.size() - MAX_count_trail));
	}

	float currentTime = engine::time::GetProgrammTime();
	while (!trail.empty() && currentTime - trail.front().timeCreation > TrailLifeTime) {
		trail.pop_front();
	}



	if (ball.path.end.y > player.GetHeight()*1.15f) {
		RespawnBall();
	}

	if (engine::time::GetProgrammTime() <= 1.f)
		return;


	if (engine::input::IsKeyPressed(KEY_Z)) {
		RespawnBall();
#ifdef _DEBUG
		path.clear();
		path.reserve(1024);
#endif
	}
#ifdef _DEBUG
	if (path.size() > 1024)
		path.erase(path.begin(), path.begin() + 1023);

	path.push_back(ball.path.begin);
#endif

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

		if (isIntersectPointPolygon(ball.path.end, border.GetVertices_OriginalBorder()) == false) {

			glm::vec2 normal;
			glm::vec2 closest = findClosestPointOnPolygon(border.GetVertices(), ball.path.end, normal);

			float len = glm::length(closest - ball.path.end);

			if (len > 0.0001f) {
				glm::vec2 dir = glm::normalize(normal);


				ball.path.begin = closest + dir * len * 0.1f;
				ball.path.end = closest + dir * len;
				ball.tangent = dir;

			}
			else {
				ball.path.end = ball.path.end - ball.tangent;

			}

		}




		if (ResolveCollision(player.GetVertices())) {
			have_collision = true;
			speedAnim = speedAnimValue;
			ball.color = glm::vec4(1.f, 0.f, 0.f, 1.f);
			player.ReactToCollision();
		}

		if (isIntersectPointPolygon(ball.path.end, player.GetVertices())) {

			glm::vec2 normal;

			glm::vec2 closest = findClosestPointOnPolygon(player.GetVertices(), ball.path.end, normal);

			float len = glm::length(closest - ball.path.end);

			if (len > 0.0001f) {
				glm::vec2 dir = glm::normalize(normal);
				len = glm::clamp(len, 0.01f, 0.1f);

				ball.path.begin = closest + dir * len * 0.1f;
				ball.path.end = closest + dir * len;
				ball.tangent = dir;				
			}
			else {
				ball.path.end = ball.path.end - ball.tangent;

			}
			speedAnim = speedAnimValue;
			ball.color = glm::vec4(1.f, 0.f, 0.f, 1.f);
			player.ReactToCollision();
		}

		for (size_t i = 0; i < objs.size(); i++) {
			if (ResolveCollision(objs[i].GetCurrentAABB(), objs[i].GetVertices())) {
				objs.erase(objs.begin() + i);
				have_collision = true;
				break;
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
	int max_recalc = 5;

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

#ifdef _DEBUG
			path.push_back(ball.path.begin);
#endif // _DEBUG
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
		ball.speed = 1.7f;

		RespawnBall();
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
