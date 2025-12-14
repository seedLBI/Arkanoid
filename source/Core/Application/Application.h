#ifndef APPLICATION_H
#define APPLICATION_H

#include "Core/Engine/Engine.h"
#include "Core/CoordinateSystem/CoordinateSystem.h"

#include "Utils/OpenGL/Enums/OpenGL.Enums.h"

class Application {
public:
	Application();
	~Application();

	void Run();

private:


	void ImGui_BeginDraw();
	void ImGui_EndDraw();

	void DrawDebugOverlay();
	void DrawBegin();

	void DrawEnd();

	void MainLoop();

};





#endif