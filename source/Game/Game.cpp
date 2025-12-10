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

	//player.Draw(quads_renderer);
	border.Draw(quads_renderer);
	border.DrawDebug(quads_renderer);
	//for (auto& obj: objs)
	//	obj.Draw(quads_renderer);

	//ball.Draw(circles_renderer);


	circles_renderer.Add(debug_begin, 5.f, glm::vec4(1.f, 0.f, 0.f, 1.f), TranslateGlobalToScreen);
	circles_renderer.Add(debug_end, 5.f, glm::vec4(0.f,1.f,0.f,1.f), TranslateGlobalToScreen);

	circles_renderer.Add(collis.position, 2.f, glm::vec4(1.f), TranslateGlobalToScreen);

}

void Game::Update() {

	if (engine::input::IsMouseDown(MOUSE_LEFT))
		debug_begin = TranslateScreenToGlobal(engine::input::GetMouseWindow());
	if (engine::input::IsMouseDown(MOUSE_RIGHT))
		debug_end = TranslateScreenToGlobal(engine::input::GetMouseWindow());


	path.clear();


	float available_length = 40.f;

	Ball ball_calc;
	ball_calc.path.begin = debug_begin;
	ball_calc.path.end = debug_end;
	ball_calc.tangent = glm::normalize(ball_calc.path.end - ball_calc.path.begin);

	while (true) {

		path.push_back(ball_calc.path.begin);

		float len_path = 0.f;
		if (path.size() > 1)
		{
			for (size_t i = 0; i < path.size() - 1; i++)
			{
				len_path += glm::length(path[i + 1] - path[i]);
			}
		}

		if (len_path > available_length || path.size() > 1024)
		{
			break;
		}

		float speed = ball.speed;
		glm::vec2& tangent = ball_calc.tangent;

		glm::vec2 nextDirection = tangent * speed * 0.1f;

		ball_calc.path.begin = ball_calc.path.end;

		ball_calc.path.end = ball_calc.path.begin + nextDirection;

		int count_recals = 0;
		int max_recalc = 6;

		while (true) {
			std::optional<CollisionInfo> collision_point;
			if (count_recals == 0)
				collision_point = GetCollision(border.GetVertices(), ball_calc.path.begin, ball_calc.path.end);
			else
				collision_point = GetCollision(border.GetVertices(), ball_calc.path.begin + ball_calc.tangent * 0.01f, ball_calc.path.end);

			if (collision_point.has_value()) {

				const CollisionInfo& collision = collision_point.value();

				collis = collision;



				float length_after_collision = glm::length(ball_calc.path.end - collision.position);

				ball_calc.tangent = collision.tangentBound;
				ball_calc.path.begin = collision.position;
				ball_calc.path.end = collision.position + collision.tangentBound * length_after_collision;


				path.push_back(ball_calc.path.begin);

			}
			else {
				break;
			}

			count_recals++;
			if (count_recals>= max_recalc)
				break;
		}


		//std::cout << count_recals << std::endl;
	}






	/*
	player.Update();

	if (engine::time::GetProgrammTime() <= 1.f)
		return;

	path.push_back(ball.path.begin);

	float deltaTime = engine::time::GetDeltaTime();
	float speed = ball.speed;
	glm::vec2& tangent = ball.tangent;

	glm::vec2 nextDirection = tangent * speed * deltaTime;

	ball.path.begin = ball.path.end;
	ball.path.end = ball.path.begin + nextDirection;



	//check collisions
	int count_check_collisions = 0;
	const int MAX_COUNT_COLLISIONS_CHECK = 6;

	while (true) {

		bool have_collision = false;

		if (ResolveCollision(border.GetVertices(),border.GetVertices_OriginalBorder())) {
			have_collision = true;
		}
		else if (ResolveCollision(player.GetVertices(), player.GetVertices_OriginalBorder())) {
			have_collision = true;
		}
		else {
			for (size_t i = 0; i < objs.size(); i++) {
				if (ResolveCollision(objs[i].GetCurrentAABB(), objs[i].GetVertices(), objs[i].GetVertices_OriginalBorder())) {
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
	*/
}



bool Game::ResolveCollision(const AABB_Region& aabb, const std::vector<glm::vec2>& vertices, const std::vector<glm::vec2>& original_vertices) {

	bool collision_have = Collision_Segment_and_AABB(ball.path, ball.radius, aabb);
	if (!collision_have) return false;

	return ResolveCollision(vertices, original_vertices);

}
bool Game::ResolveCollision(const std::vector<glm::vec2>& vertices, const std::vector<glm::vec2>& original_vertices) {

	auto collision_point = GetCollision(vertices, ball.path.begin, ball.path.end);

	if (collision_point.has_value()) {
		const CollisionInfo& collision = collision_point.value();

		float error_length = glm::length(collision.position - ball.path.begin);
		const float eps = 1e-6f;


		float length_after_collision = glm::length(ball.path.end - collision.position);

		if (error_length < eps) {
			auto collision_point_in_original_border = GetCollision(original_vertices, ball.path.begin, ball.path.end);
			if (collision_point_in_original_border.has_value()) {

				ball.tangent = collision_point_in_original_border.value().tangentBound;
				ball.path.begin = collision.position;
				ball.path.end = collision.position + ball.tangent * length_after_collision;

				return true;
			}

			return false;
		}

		ball.tangent = collision.tangentBound;
		ball.path.begin = collision.position;
		ball.path.end = collision.position + collision.tangentBound * length_after_collision;
		return true;
	}

	return false;

}


void Game::Load(const nlohmann::json& dataLevel) {
	this->data = dataLevel;

	if (data.contains("ballSpawn")) {
		ballSpawn.Load(data["ballSpawn"]);

		ball.radius = ballSpawn.global_radius;
		ball.speed = 34.7f;
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
