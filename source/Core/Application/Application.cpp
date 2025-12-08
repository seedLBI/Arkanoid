#include "Application.h"

#include "Rendering/RenderObjects/DebugLine/Render.Debug.Line.h"
#include "Rendering/RenderObjects/DebugCircle/Render.Debug.Circle.h"
#include "Rendering/RenderObjects/QuadInstanced/Render.Quad.Instanced.h"	

#include "Game/LevelBorder/LevelBorder.h"
#include "Game/Segment/Segment.h"
#include "Game/Math/Arkanoid_Math.h"
#include "Game/Player/Player.h"

#include "Game/LevelCreator/LevelCreator.h"


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
	QuadInstanced quads;

	float global_radius = 0.05f;
	float screen_radius = TranslateScalar_GlobalToScreen(0.05f);


	LevelBorder borderBox(
		{
			0.5f * glm::vec2(-1, 1),
			0.5f * glm::vec2(1, 1),
			0.5f * glm::vec2(0.5f, 0.f),
			0.5f * glm::vec2(1, -1),
			0.5f * glm::vec2(-1, -1),
			0.5f * glm::vec2(-0.5f, 0.f),
		},
		global_radius
	);

	glm::vec2 start_pos = { 0,0 };

	Player player;

	LevelCreator levelCreator;

	while (!engine::window::IsShouldClose()) {
		DrawBegin();
		//player.Update();
		levelCreator.Update();

		glm::vec2 mouse_global = TranslateScreenToGlobal(engine::input::GetMouseWindow());

		if (engine::input::IsKeyPressed(KeyboardButton::KEY_ESCAPE))
			engine::window::Close();

		if (engine::input::IsKeyPressed(KeyboardButton::KEY_F11))
			engine::window::ToggleWindowState();

		if (engine::input::IsKeyPressed(KeyboardButton::KEY_F10))
			engine::window::ToggleVSync();

		if (engine::input::IsMousePressed(MOUSE_RIGHT))
			start_pos = mouse_global;


		levelCreator.Draw(circles,quads);




		/*
		borderBox.Draw(quads);
		player.Draw(quads);

		
		
		auto collision = GetCollision(borderBox.GetVertices(), start_pos, mouse_global);
		glm::vec2 pos_bound;
		glm::vec2 prev_collision;
		
		if (collision.has_value()) {

			const CollisionInfo& info = collision.value();

			float length_path_after_collision = glm::length(info.position - mouse_global);

			pos_bound = info.position + info.tangentBound * length_path_after_collision;
			prev_collision = info.position + info.tangentBound * 0.01f;

			quads.AddLine(start_pos, info.position, COLOR_RED, TranslateGlobalToScreen);
			quads.AddLine(mouse_global, info.position,COLOR_BLUE, TranslateGlobalToScreen);
			quads.AddLine(info.position, pos_bound, COLOR_GREEN, TranslateGlobalToScreen);

			collision.reset();
			int index = 0;
			while(true)
			{
				auto next_collision = GetCollision(borderBox.GetVertices(), prev_collision, pos_bound);
				if (next_collision.has_value()) {

					const CollisionInfo& next_info = next_collision.value();

					circles.Add(next_info.position, screen_radius, COLOR_GREEN, TranslateGlobalToScreen);

					length_path_after_collision = glm::length(next_info.position - pos_bound);

					pos_bound = next_info.position + length_path_after_collision * next_info.tangentBound;

					quads.AddLine(prev_collision, next_info.position, COLOR_GREEN, TranslateGlobalToScreen);
					prev_collision = next_info.position + 0.001f * next_info.tangentBound ;
				}
				else {
					break;
				}
				index++;
				if (index >= 10) break;



			}
			

		}
		
		


		circles.Add(mouse_global, 20.f, glm::vec4(1.f,0.f,0.f,0.2f), TranslateGlobalToScreen);
		circles.Add(start_pos, 10.f, COLOR_GREEN, TranslateGlobalToScreen);
		quads.AddRectangle(mouse_global, { 20.f,20.f }, glm::vec4(1.f), TranslateGlobalToScreen);

		lines.Render();
		quads.Render();
		circles.Render();

		*/


		//lines.Render();
		quads.Render();
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


