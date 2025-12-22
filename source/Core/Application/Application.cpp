#include "Application.h"

#include "Rendering/RenderObjects/DebugLine/Render.Debug.Line.h"
#include "Rendering/RenderObjects/DebugCircle/Render.Debug.Circle.h"
#include "Rendering/RenderObjects/QuadInstanced/Render.Quad.Instanced.h"	
#include "Rendering/RenderObjects/TriangleInstanced/Render.Triangle.Instanced.h"
#include "Rendering/RenderObjects/TextInstanced/Render.TextInstanced.h"

#include "Game/LevelCreator/LevelCreator.h"
#include "Game/Game.h"

#include "Core/Colors/Colors.h"

#include <tuple>


Application::Application() {
	engine::core::Initialize();

	engine::time::SetTargetFPS(0);
	//engine::window::EnableVSync();
	//engine::window::SetFullscreen();
}
Application::~Application() {
	engine::core::Terminate();
}

void Application::MainLoop() {
	// renderer
	DebugLine		  lines;
	DebugCircle		  circles;
	QuadInstanced	  quads;
	TriangleInstanced triangles;
	TextInstanced	  texts;


	FontAtlas font;


	LevelCreator levelCreator;
	Game game;
	
	
	font.Load("resources/fonts/VCROSD");
	texts.AttachTexture(font.getTexture());
	game.AttachFont(&font);



	std::ifstream ifn("resources/levels/TightCorners.level");
		game.Load(nlohmann::json::parse(ifn));
	ifn.close();

	


	glfwSetTime(0.0);


	TextAtlas test = makeText(u8"Дарова поворот!", glm::vec4(1.f), glm::vec4(1.f));


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

		game.Update();
		game.Draw(triangles, quads, circles, texts);

		//game.UpdateDebug();
		//game.DrawDebug(triangles, quads, circles);
		

		triangles.Render();
		circles.Render();
		quads.Render();
		texts.Render();

#ifdef _DEBUG
		DrawDebugOverlay();
#endif
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



	glm::vec2 mouse_window_screen = engine::input::GetMouseWindow();
	glm::vec2 mouse_window_global = TranslateScreenToGlobal(engine::input::GetMouseWindow());
	glm::vec2 mouse_screen_screen = engine::input::GetMouseScreen();
	glm::vec2 mouse_screen_global = TranslateScreenToGlobal(engine::input::GetMouseScreen());

	ImGui::Text("Mouse window (screen): (%f, %f)", mouse_window_screen.x, mouse_window_screen.y);
	ImGui::Text("Mouse window (global): (%f, %f)", mouse_window_global.x, mouse_window_global.y);
	ImGui::Text("Mouse screen (screen): (%f, %f)", mouse_screen_screen.x, mouse_screen_screen.y);
	ImGui::Text("Mouse screen (global): (%f, %f)", mouse_screen_global.x, mouse_window_global.y);

	ImGui::End();
}


void Application::DrawBegin() {
#ifdef _DEBUG
	ImGui_BeginDraw();
#endif
	engine::core::UpdateFrame();
	glfwPollEvents();

	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(BufferBit::COLOR_BUFFER_BIT | BufferBit::DEPTH_BUFFER_BIT);

}
void Application::DrawEnd() {
#ifdef _DEBUG
	ImGui_EndDraw();
#endif
	glfwSwapBuffers(engine::window::GetHandle());
	engine::time::LimitFrameRate();
}


void Application::Run() {
	MainLoop();
}


