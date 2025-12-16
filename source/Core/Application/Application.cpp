#include "Application.h"

#include "Rendering/RenderObjects/DebugLine/Render.Debug.Line.h"
#include "Rendering/RenderObjects/DebugCircle/Render.Debug.Circle.h"
#include "Rendering/RenderObjects/QuadInstanced/Render.Quad.Instanced.h"	
#include "Rendering/RenderObjects/TriangleInstanced/Render.Triangle.Instanced.h"


#include "Game/LevelCreator/LevelCreator.h"
#include "Game/Game.h"



#include "Core/Colors/Colors.h"


Application::Application() {
	engine::core::Initialize();

	//engine::window::EnableVSync();
	engine::time::SetTargetFPS(0);
	//engine::window::SetFullscreen();
}
Application::~Application() {
	engine::core::Terminate();
}

void Application::MainLoop() {
	DebugLine lines;
	DebugCircle circles;
	QuadInstanced quads;
	TriangleInstanced triangles;

	LevelCreator levelCreator;

	Game game;

	std::ifstream ifn("resources/levels/TightCorners.level");
	nlohmann::json data_level = nlohmann::json::parse(ifn);
	ifn.close();

	game.Load(data_level);


	glfwSetTime(0.0);

	while (!engine::window::IsShouldClose()) {
		DrawBegin();

		if (engine::input::IsKeyPressed(KeyboardButton::KEY_ESCAPE))
			engine::window::Close();

		if (engine::input::IsKeyPressed(KeyboardButton::KEY_F11))
			engine::window::ToggleWindowState();

		if (engine::input::IsKeyPressed(KeyboardButton::KEY_F10))
			engine::window::ToggleVSync();

		//levelCreator.Update();
		//levelCreator.Draw(circles, quads,triangles);

		//game.Update();
		//game.Draw(triangles, quads, circles);

		game.UpdateDebug();
		game.DrawDebug(triangles, quads, circles);


		quads.Render();
		circles.Render();
		triangles.Render();

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
	ImGui::Text("Time: %f", engine::time::GetProgrammTime());
	ImGui::Text("DeltaTime: %f", engine::time::GetDeltaTime());
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


