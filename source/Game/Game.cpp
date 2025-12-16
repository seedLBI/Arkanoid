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


	for (auto& obj : objs)
		obj.DrawDebug(quads_renderer);
	player.Draw(quads_renderer);
	player.DrawDebug(quads_renderer);

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
	player.Update();

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


		ResolveCollision(debug_ball);

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

	ResolveCollision(ball);
}


void Game::SetNextPosition(Ball& ball_, const glm::vec2 tangent, const  glm::vec2 begin, const  glm::vec2 end) {
	ball_.tangent = tangent;
	ball_.path.begin = begin;
	ball_.path.end = end;

	debug_path.push_back(ball_.path.begin);
}

std::optional<CollisionInfo> Game::TryCollision(Ball& ball_, const AABB_Region& aabb, const std::vector<glm::vec2>& vertices, bool alwaysInside) {

	bool collision_have = Collision_Segment_and_AABB(ball_.path, ball_.radius, aabb);
	if (!collision_have) return std::nullopt;

	return TryCollision(ball_, vertices, alwaysInside);

}

std::optional<CollisionInfo> Game::TryCollision(Ball& ball_, const std::vector<glm::vec2>& vertices, bool alwaysInside) {
	return GetCollision(
		vertices,
		ball_.path.begin,
		ball_.path.end,
		alwaysInside
	);
}

Game::ClosestCollisionData Game::GetClosestCollision(
	Ball& ball_,
	const std::vector<std::optional<CollisionInfo>>& collision_destroyable,
	const std::optional<CollisionInfo>& collision_player,
	const std::optional<CollisionInfo>& collision_border
) {
	ClosestCollisionData output;
	output.index = 0;
	output.object = COLLISION_OBJECT_TYPE::COLLISION_NOTHING;
	output.info = std::nullopt;

	float length_min = std::numeric_limits<float>::max();

	auto calc_length = [&](const CollisionInfo& collision) ->float {
		return glm::length(ball_.path.begin - collision.position);
	};

	if (collision_player.has_value()) {
		float len = calc_length(collision_player.value());
		if (len < length_min) {
			length_min = len;
			output.object = COLLISION_OBJECT_TYPE::COLLISION_PLAYER;
			output.info = collision_player;
		}
	}

	if (collision_border.has_value()) {
		float len = calc_length(collision_border.value());
		if (len < length_min) {
			length_min = len;
			output.object = COLLISION_OBJECT_TYPE::COLLISION_BORDER;
			output.info = collision_border;
		}
	}

	for (size_t i = 0; i < collision_destroyable.size(); i++) {

		if (collision_destroyable[i].has_value()) {
			float len = calc_length(collision_destroyable[i].value());
			if (len < length_min) {
				length_min = len;
				output.object = COLLISION_OBJECT_TYPE::COLLISION_DESTROYABLE;
				output.index = i;
				output.info = collision_destroyable[i];
			}
		}
	}

	return output;
}

bool Game::ResolveCollision(Ball& ball_) {

	int count_recals = 0;
	const int max_recalc = 100;

	while (count_recals <= max_recalc) {

		std::vector<std::optional<CollisionInfo>> collision_objects;
		collision_objects.reserve(objs.size());
		for (auto& obj : objs)
			collision_objects.push_back(
								 TryCollision(ball_, obj.GetCurrentAABB(), obj.GetVertices(), COLLISION_PUSH_TYPE::ALWAYS_PUSH_OUTSIDE));
		auto collision_player  = TryCollision(ball_, player.GetCurrentAABB(), player.GetVertices(), COLLISION_PUSH_TYPE::ALWAYS_PUSH_OUTSIDE);
		auto collision_border  = TryCollision(ball_, border.GetVertices(), COLLISION_PUSH_TYPE::ALWAYS_PUSH_INSIDE);

		auto collisionData = GetClosestCollision(ball_, collision_objects, collision_player, collision_border);

		if (collisionData.info.has_value()) {
			const CollisionInfo& collision = collisionData.info.value();

			float length_after_collision = glm::length(ball_.path.end - collision.position);

			if (length_after_collision < 1e-4f) {
				SetNextPosition(ball_,
					collision.tangentBound,
					collision.position + collision.tangentBound * 0.0001f,
					collision.position + collision.tangentBound * 0.001f
				);
			}
			else {
				SetNextPosition(ball_,
					collision.tangentBound,
					collision.position + collision.tangentBound * 0.0001f,
					collision.position + collision.tangentBound * length_after_collision
				);
			}


			//ReactToCollision();

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

void Game::ReactToCollision(const ClosestCollisionData& data){
	switch (data.object) {

	case COLLISION_OBJECT_TYPE::COLLISION_PLAYER:
		speedAnim = speedAnimValue;
		ball.color = glm::vec4(1.f, 0.f, 0.f, 1.f);
		player.ReactToCollision();
		break;


	case COLLISION_OBJECT_TYPE::COLLISION_DESTROYABLE:
		objs[data.index].SetDamage(25.f);
		if (objs[data.index].IsShouldDelete())
			objs.erase(objs.begin() + data.index);
		break;


	default:
		break;
	}
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
