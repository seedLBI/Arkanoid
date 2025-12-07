#include "Application.h"

#include "Rendering/RenderObjects/DebugLine/Render.Debug.Line.h"
#include "Rendering/RenderObjects/DebugCircle/Render.Debug.Circle.h"

#include "Game/LevelBorder/LayerBorder.h"

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

	LayerBorder borderBox(
		{
			0.5f * glm::vec2(-1, 1),
			0.5f * glm::vec2(1, 1),
			0.5f * glm::vec2(0.5f, 0.f),
			0.5f * glm::vec2(1, -1),
			0.5f * glm::vec2(-1, -1),
			0.5f * glm::vec2(-0.5f, 0.f),
			0.5f * glm::vec2(-1, 1)
		}
	);

	while (!engine::window::IsShouldClose()) {
		DrawBegin();


		if (engine::input::IsKeyPressed(KeyboardButton::KEY_ESCAPE))
			engine::window::Close();

		if (engine::input::IsKeyPressed(KeyboardButton::KEY_F11))
			engine::window::ToggleWindowState();

		if (engine::input::IsKeyPressed(KeyboardButton::KEY_F10))
			engine::window::ToggleVSync();


		borderBox.Draw(lines);

		glm::vec2 mouse_global = TranslateScreenToGlobal(engine::input::GetMouseWindow());

		auto collision = borderBox.GetCollision(glm::vec2(0, 0), mouse_global);


		if (collision.has_value()) {

			const CollisionInfo& info = collision.value();
			circles.Add(info.position, 20.f, COLOR_GREEN, TranslateGlobalToScreen);

			float length_path_after_collision = glm::length(mouse_global - info.position);

			glm::vec2 pos_bound = info.position + info.tangentBound * length_path_after_collision;

			glm::vec2 prev_collision = info.position + info.tangentBound * 0.001f;

			lines.Add({0.f,0.f}, info.position, COLOR_RED, TranslateGlobalToScreen);
			lines.Add(mouse_global, info.position,COLOR_BLUE, TranslateGlobalToScreen);
			lines.Add(info.position, pos_bound, COLOR_GREEN, TranslateGlobalToScreen);
		}

		


		circles.Add(engine::input::GetMouseWindow(), 20.f, COLOR_RED);


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


