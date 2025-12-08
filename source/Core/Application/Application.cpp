#include "Application.h"

#include "Rendering/RenderObjects/DebugLine/Render.Debug.Line.h"
#include "Rendering/RenderObjects/DebugCircle/Render.Debug.Circle.h"

#include "Game/LevelBorder/LevelBorder.h"
#include "Game/Segment/Segment.h"
#include "Game/Math/Arkanoid_Math.h"


#include "Core/Colors/Colors.h"


Application::Application() {
	engine::core::Initialize();

	//engine::window::EnableVSync();
	//engine::window::SetFullscreen();
}
Application::~Application() {
	engine::core::Terminate();
}

void Application::MainLoop() {
	DebugLine lines;
	DebugCircle circles;

	LevelBorder borderBox(
		{
			0.5f * glm::vec2(-1, 1),
			0.5f * glm::vec2(1, 1),
			0.5f * glm::vec2(0.5f, 0.f),
			0.5f * glm::vec2(1, -1),
			0.5f * glm::vec2(-1, -1),
			0.5f * glm::vec2(-0.5f, 0.f),
		}
	);

	glm::vec2 start_pos = { 0,0 };


	while (!engine::window::IsShouldClose()) {
		DrawBegin();

		glm::vec2 mouse_global = TranslateScreenToGlobal(engine::input::GetMouseWindow());



		if (engine::input::IsKeyPressed(KeyboardButton::KEY_ESCAPE))
			engine::window::Close();

		if (engine::input::IsKeyPressed(KeyboardButton::KEY_F11))
			engine::window::ToggleWindowState();

		if (engine::input::IsKeyPressed(KeyboardButton::KEY_F10))
			engine::window::ToggleVSync();

		if (engine::input::IsMousePressed(MOUSE_RIGHT))
			start_pos = mouse_global;


		borderBox.Draw(lines);

		/*
		const auto& vertices = borderBox.GetVertices();
		Segment ball_path{ start_pos, mouse_global };

		std::vector<std::pair<size_t, glm::vec2>> collisions;

		for (size_t i = 0; i < vertices.size() - 1; i++) {
			Segment border{ vertices[i], vertices[i + 1] };

			float time = getTimeCollisionBetweenTwoSegment(border, ball_path);
			float time2 = getTimeCollisionBetweenTwoSegment(ball_path, border);

			if (time > 1.f || time < 0.f || time2 > 1.f || time2 < 0.f)
				continue;

			glm::vec2 point = lerp(border, time);
			collisions.push_back({ i, point });
		}

		std::sort(collisions.begin(), collisions.end(), [&start_pos](const auto& a, const auto& b) {
			return glm::length(a.second - start_pos) > glm::length(b.second - start_pos);
		});
		if (collisions.empty() == false)
		{
			circles.Add(collisions.back().second, 10.f, COLOR_GREEN, TranslateGlobalToScreen);
		}



		for (size_t i = 0; i < collisions.size(); i++)
		{
			circles.Add(collisions[i].second, 5.f, COLOR_RED, TranslateGlobalToScreen);
		}
		*/

		
		auto collision = borderBox.GetCollision(start_pos, mouse_global);
		glm::vec2 pos_bound;
		glm::vec2 prev_collision;
		
		if (collision.has_value()) {

			const CollisionInfo& info = collision.value();

			float length_path_after_collision = glm::length(info.position - mouse_global);

			pos_bound = info.position + info.tangentBound * length_path_after_collision;
			prev_collision = info.position + info.tangentBound * 0.01f;

			lines.Add(start_pos, info.position, COLOR_RED, TranslateGlobalToScreen);
			lines.Add(mouse_global, info.position,COLOR_BLUE, TranslateGlobalToScreen);
			lines.Add(info.position, pos_bound, COLOR_GREEN, TranslateGlobalToScreen);

			collision.reset();
			int index = 0;
			while(true)
			{
				auto next_collision = borderBox.GetCollision(prev_collision, pos_bound);
				if (next_collision.has_value()) {

					const CollisionInfo& next_info = next_collision.value();


					float radius = abs( TranslateGlobalToScreen(glm::vec2{ 0.05f,0.f }).x - TranslateGlobalToScreen(glm::vec2{ 0.f,0.f }).x);

					circles.Add(next_info.position, radius, COLOR_GREEN, TranslateGlobalToScreen);

					length_path_after_collision = glm::length(next_info.position - pos_bound);

					pos_bound = next_info.position + length_path_after_collision * next_info.tangentBound;

					lines.Add(prev_collision, next_info.position, COLOR_GREEN, TranslateGlobalToScreen);
					prev_collision = next_info.position + 0.001f * next_info.tangentBound ;
				}
				else {
					break;
				}
				index++;
				if (index >= 10) break;



			}
			

		}

		


		circles.Add(mouse_global, 20.f, COLOR_RED, TranslateGlobalToScreen);
		circles.Add(start_pos, 10.f, COLOR_GREEN, TranslateGlobalToScreen);


		lines.Render();
		circles.Render();
		DrawDebugOverlay();
		DrawEnd();
	}
}





void Application::ImGui_BeginDraw() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}
void Application::ImGui_EndDraw() {
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::DrawDebugOverlay() {
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Appearing);
	ImGui::Begin("##Master Dream", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar);
	ImGui::Text("FPS: %i", (int)ImGui::GetIO().Framerate);
	glm::vec2 m = engine::input::GetMouseWindow();
	ImGui::Text("Mouse_Window (%i,%i)", (int)m.x, (int)m.y);
	glm::vec2 mg = TranslateScreenToGlobal(engine::input::GetMouseWindow());
	ImGui::Text("Mouse_Global (%f,%f)", mg.x, mg.y);
	ImGui::End();
}


void Application::DrawBegin() {
	ImGui_BeginDraw();
	engine::core::UpdateFrame();
	glfwPollEvents();

	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(BufferBit::COLOR_BUFFER_BIT | BufferBit::DEPTH_BUFFER_BIT);

}
void Application::DrawEnd() {
	ImGui_EndDraw();
	glfwSwapBuffers(engine::window::GetHandle());
	engine::time::LimitFrameRate();
}


void Application::Run() {
	MainLoop();
}


