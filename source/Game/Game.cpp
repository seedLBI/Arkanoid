#include "Game.h"

#include "Game/Math/Arkanoid_Math.h"
#include "Core/CoordinateSystem/CoordinateSystem.h"

#include <imgui.h>

Game::Game() {

}

Game::~Game() {

}


void Game::DrawDebug(
	TriangleInstanced& triangles_renderer, 
	QuadInstanced&     quads_renderer, 
	DebugCircle&       circles_renderer) {




	if (ImGui::Begin("Debug path tracer")){

		if (ImGui::RadioButton("PathTracer", debug_flag_enable_path_tracer)) {
			debug_flag_enable_path_tracer = !debug_flag_enable_path_tracer;
		}

		ImGui::Text("pathPoints = %i", (int)debug_path.size());
		ImGui::Text("deltaTime = %f", debug_deltaTime);
		ImGui::SliderFloat("FPS", &debug_FPS, 1.f, 300.f);
		ImGui::SliderFloat("Speed", &debug_speed, 0.01f, 3.f);
		ImGui::SliderFloat("LengthTrace", &debug_length_path, 0.01f, 24.f);


		ImGui::End();
	}


	//for (auto& obj : objs) {
	//	obj.DrawDebug(quads_renderer);
	//}

	//player.Draw(quads_renderer);
	//player.DrawDebug(quads_renderer);

	border.Draw(quads_renderer);
	border.DrawDebug(quads_renderer);


	// dragable circles
	circles_renderer.Add(debug_ball_begin, 10.f, glm::vec4(1.f, 0.f, 0.f, 0.5f), TranslateGlobalToScreen);
	circles_renderer.Add(debug_ball_end, 10.f, glm::vec4(0.f, 0.f, 1.f, 0.5f), TranslateGlobalToScreen);


	// direction arrow
	quads_renderer.AddLine(debug_ball_begin, debug_ball_end, 2.f, glm::vec4(0.f, 1.f, 0.f, 1.f), TranslateGlobalToScreen);

	glm::vec2 perp = perp_normalized(getDirection(debug_ball_begin, debug_ball_end));
	quads_renderer.AddLine(debug_ball_end, debug_ball_end - rotate(perp, glm::pi<float>() / 3.f) * 0.05f, 2.f, glm::vec4(0.f, 1.f, 0.f, 1.f), TranslateGlobalToScreen);
	quads_renderer.AddLine(debug_ball_end, debug_ball_end - rotate(-perp, -glm::pi<float>() / 3.f) * 0.05f, 2.f, glm::vec4(0.f, 1.f, 0.f, 1.f), TranslateGlobalToScreen);


	// test collision
	auto test_collision = GetCollision(border.GetVertices(), debug_ball_begin, debug_ball_end);
	if (test_collision.has_value()) {
		const auto& collision_value = test_collision.value();

		const glm::vec2 begin = collision_value.position;
		const glm::vec2 end = collision_value.position + collision_value.tangentBound * 0.1f;

		circles_renderer.Add(begin, 5.f, glm::vec4(1.f, 0.f, 0.f, 1.f), TranslateGlobalToScreen);
		circles_renderer.Add(end, 5.f, glm::vec4(1.f, 0.f, 0.f, 1.f), TranslateGlobalToScreen);


		quads_renderer.AddLine(begin, end, 2.f, glm::vec4(0.3f, 0.3f, 1.f, 1.f), TranslateGlobalToScreen);

	}






	if (debug_flag_enable_path_tracer) {
		circles_renderer.Add(debug_ball.path.end, 5.f, glm::vec4(0.1f, 0.1f, 1.f, 1.f), TranslateGlobalToScreen);
		quads_renderer.AddLine(debug_ball.path.begin, debug_ball.path.end, 2.f, glm::vec4(0.3f, 0.3f, 1.f, 1.f), TranslateGlobalToScreen);

		// path
		if (debug_path.empty()) return;

		circles_renderer.Add(debug_path[0], 4.f, glm::vec4(1.f, 0.f, 0.f, 1.f), TranslateGlobalToScreen);
		for (size_t i = 1; i < debug_path.size(); i++) {
			quads_renderer.AddLine(debug_path[i - 1], debug_path[i], 2.f, glm::vec4(1.f, 1.f, 0.f, 1.f), TranslateGlobalToScreen);
			circles_renderer.Add(debug_path[i], 4.f, glm::vec4(1.f, 0.f, 0.f, 1.f), TranslateGlobalToScreen);
		}
	}

}

void Game::UpdateDebugInput() {
	glm::vec2 global_mouse = TranslateScreenToGlobal(engine::input::GetMouseWindow());


	static bool drag_begin = false;
	static bool drag_end = false;

	if (Collision_Point_and_Circle(global_mouse, debug_ball_begin, TranslateScalar_ScreenToGlobal(10.f))) {
		if (engine::input::IsMousePressed(MOUSE_LEFT)) {
			drag_begin = true;
			drag_end = false;
		}
	}
	else if (Collision_Point_and_Circle(global_mouse, debug_ball_end, TranslateScalar_ScreenToGlobal(10.f))) {
		if (engine::input::IsMousePressed(MOUSE_LEFT)) {
			drag_end = true;
			drag_begin = false;
		}
	}

	if (drag_begin) {
		if (engine::input::IsMouseDown(MOUSE_LEFT))
			debug_ball_begin = global_mouse;
		else
			drag_begin = false;

	}
	else if (drag_end) {
		if (engine::input::IsMouseDown(MOUSE_LEFT))
			debug_ball_end = global_mouse;
		else
			drag_end = false;
	}
}

float Game::GetLengthPath() {
	float length_path = 0.f;
	for (size_t i = 1; i < debug_path.size(); i++) {
		length_path += glm::length(debug_path[i - 1] - debug_path[i]);
	}
	return length_path;
}

void Game::UpdateDebug() {
	UpdateDebugInput();
	//player.Update();

	if (!debug_flag_enable_path_tracer) {

		if (debug_path.empty() == false)
			debug_path.clear();

		return;
	}

	debug_path.erase(debug_path.begin(), debug_path.end());
	debug_path.clear();
	debug_path.reserve(1000);


	debug_ball.speed = debug_speed;
	debug_ball.path.begin = debug_ball_begin;
	debug_ball.path.end   = debug_ball_end;
	debug_ball.tangent    = getDirection(debug_ball_begin, debug_ball_end);


	while (true) {


		if (GetLengthPath() > debug_length_path)
			return;

		if (debug_path.size() > 1000)
			return;

		debug_deltaTime = 1.f / debug_FPS;

		glm::vec2 nextDirection = debug_ball.tangent * debug_speed * debug_deltaTime;

		SetNextPosition(debug_ball,
			debug_ball.tangent,
			debug_ball.path.end,
			debug_ball.path.end + nextDirection);


		int count_check_collisions = 0;
		const int MAX_COUNT_COLLISIONS_CHECK = 1;

		
		while (true) {

			bool have_collision = false;

			if (isnan(debug_ball.path.begin.x))
			{
				std::cout << "nan\n";
				return;
			}

			//if (ResolveCollision(player.GetVertices(), debug_ball)) {
			//	have_collision = true;
			//}

			if (ResolveCollision(border.GetVertices(), debug_ball)) {
				have_collision = true;
			}
			/*
			if (isIntersectPointPolygon(debug_ball.path.end, border.GetVertices()) == false) {
				std::cout << "inside!!!\n";

				glm::vec2 normal;
				glm::vec2 closest = findClosestPointOnPolygon(border.GetVertices(), debug_ball.path.end, normal);
				glm::vec2 dir = glm::normalize(glm::reflect(debug_ball.tangent, normal));

				float len = glm::length(closest - debug_ball.path.end);

				SetNextPosition(debug_ball, dir, closest + dir * len * 0.1f, closest + dir * len);
			}
			*/



			if (GetLengthPath() > debug_length_path)
				return;

			if (debug_path.size() > 1000)
				return;


			if (have_collision) {
				count_check_collisions++;
			}
			else
				break;

			if (count_check_collisions >= MAX_COUNT_COLLISIONS_CHECK)
				break;

		}
		

	}

}



void Game::Draw(TriangleInstanced& triangles_renderer, QuadInstanced& quads_renderer, DebugCircle& circles_renderer) {

#ifdef _DEBUG
	player.DrawDebug(quads_renderer);
	border.DrawDebug(quads_renderer);
	for (auto& obj : objs) {
		obj.DrawDebug(quads_renderer);
	}
#endif

	player.Draw(quads_renderer);
	border.Draw(quads_renderer);

	for (auto& obj : objs) {
		obj.Draw(quads_renderer, triangles_renderer);
	}

	for (size_t i = 0; i < trail.size(); i++) {
		float time = (float)i / (float)trail.size();

		const glm::vec2& pos = trail[i].pos;
		float radius = TranslateScalar_GlobalToScreen(ball.radius) * time;
		glm::vec4 color = ball.color + (1.f - time) * (glm::vec4(0.f, 0.f, 0.f, 0.f) - ball.color) ;

		circles_renderer.Add(pos, radius, color, TranslateGlobalToScreen);
	}

	quads_renderer.AddLine(ball.path.begin, ball.path.end, glm::vec4(0.f, 0.f, 1.f, 1.f), TranslateGlobalToScreen);


	ball.Draw(circles_renderer);
}

void Game::RespawnBall() {
	ball.tangent = glm::normalize(ballSpawn.tangent);
	ball.path.begin = ballSpawn.globalPos;
	ball.path.end = ballSpawn.globalPos + glm::normalize(ballSpawn.tangent) * ball.radius;
	trail.clear();
}

void Game::TrailUpdate() {
	if (trail.empty() == false) {
		float len_last = glm::length(ball.path.begin - trail.back().pos);

		if (!isnan(len_last)) {

			const float len_need = LengthTrail / (float)MAX_count_trail;

			int countAdd = len_last / len_need;
			const glm::vec2 begin = ball.path.begin;
			const glm::vec2 end = trail.back().pos;
			for (int i = countAdd - 1; i >= 0; i--) {

				float time = (len_need * (float)i) / len_last;

				glm::vec2 pointAdd = lerp(begin, end, time);

				trail.push_back({ pointAdd, engine::time::GetProgrammTime() });
			}
		}
	}
	else {
		trail.push_back({ ball.path.begin,engine::time::GetProgrammTime() });
	}


	for (size_t i = 0; i < trail.size(); i++) {
		auto& p = trail[i];
		const float currentTime = engine::time::GetProgrammTime();

		if (currentTime - p.timeCreation > LifeTimePoints) {
			trail.erase(trail.begin() + i);
			i--;
		}
	}
	if (trail.size() > MAX_count_trail) {
		trail.erase(trail.begin() , trail.end() - MAX_count_trail);
	}
}

void Game::UpdateAnimValues() {
	speedAnim = speedAnim + engine::time::GetDeltaTime() * (0.f - speedAnim);
	ball.color = ball.color + engine::time::GetDeltaTime() * 2.f * (glm::vec4(1.f, 1.f, 1.f, 1.f) - ball.color);

	ball.speed = 1.f + speedAnim;

}

void Game::Update() {
	UpdateAnimValues();
	TrailUpdate();

	player.Update();

	if (ball.path.end.y > player.GetHeight() * 1.05f) {
		RespawnBall();
	}

	if (engine::time::GetProgrammTime() <= 1.f)
		return;


	if (engine::input::IsKeyPressed(KEY_Z)) {
		RespawnBall();
	}


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



		if (ResolveCollision(border.GetVertices(), ball, true)) {
			have_collision = true;
		}

		if (ResolveCollision(player.GetVertices(), ball, false)) {
			have_collision = true;
			speedAnim = speedAnimValue;
			ball.color = glm::vec4(1.f, 0.f, 0.f, 1.f);
			player.ReactToCollision();
		}

		bool WasCollision = true;

		while (WasCollision) {
			WasCollision = false;
			for (size_t i = 0; i < objs.size(); i++) {
				if (ResolveCollision(objs[i].GetCurrentAABB(), objs[i].GetVertices(), ball, false)) {
					
					objs[i].SetDamage(20.f);

					if (objs[i].IsShouldDelete()) {
						objs.erase(objs.begin() + i);
					}


					WasCollision = true;
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


void Game::SetNextPosition(Ball& ball_,const glm::vec2 tangent, const  glm::vec2 begin, const  glm::vec2 end) {
	ball_.tangent    = tangent;
	ball_.path.begin = begin;
	ball_.path.end   = end;

	//debug_path.push_back(ball_.path.begin);
}

bool Game::ResolveCollision(const AABB_Region& aabb, const std::vector<glm::vec2>& vertices, Ball& ball_, bool alwaysInside ) {

	bool collision_have = Collision_Segment_and_AABB(ball.path, ball.radius, aabb);
	if (!collision_have) return false;

	return ResolveCollision(vertices, ball_, alwaysInside);

}
bool Game::ResolveCollision(const std::vector<glm::vec2>& vertices, Ball& ball_, bool alwaysInside) {

	int count_recals = 0;
	int max_recalc = 100;

	//std::cout << std::endl;


	while (true) {
		std::optional<CollisionInfo> collision_point;

		if (count_recals == 0)
			collision_point = GetCollision(vertices, ball_.path.begin, ball_.path.end, alwaysInside);
		else
			collision_point = GetCollision(vertices, ball_.path.begin + ball_.tangent * 0.001f, ball_.path.end, alwaysInside);

		if (collision_point.has_value()) {

			const CollisionInfo& collision = collision_point.value();

			float length_after_collision = glm::length(ball_.path.end - collision.position);

			SetNextPosition(ball_,
				collision.tangentBound,
				collision.position,
				collision.position + collision.tangentBound * length_after_collision
			);

			if (GetLengthPath() > debug_length_path || debug_path.size() > 1000)
				return true;
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
		ball.speed = 0.7f;

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
