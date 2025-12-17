#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include <cstdint>
#include <string>
#include <optional>

#define GLEW_STATIC
#include <GL/glew.h>

#define  GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <nfd_glfw3.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <glm/glm.hpp>

#include "Engine.Keyboard.Enums.h"
#include "Engine.Mouse.Enums.h"

#include "Utils/Timer/Timer.Framerate.h"

#include "Utils/OpenGL/UBO/UBO.h"
#include "Core/UBOs/MainBlock.h"




namespace engine {

	namespace core {

		namespace vars {
			extern GLFWwindow* handle_window;
			extern FPS_Timer* fps_limiter;


			extern glm::vec2 view_translate;
			extern glm::vec2 view_scale;
			extern float view_rotate_angle;

			extern MainBlock mainBlock;
			extern UBO<MainBlock>* ubo_mainBlock;

			extern std::vector<GLFWcursor*> cursors;
			extern int currentCursor;


			extern glm::vec2 sizeFramebuffer;
			extern float minSizeFramebuffer;
			extern glm::ivec2 last_pos_window;
			extern glm::ivec2 last_size_window;
			extern int window_state;

			extern bool VSync_state;

		}

		void Initialize();
		void Terminate();

		void InitGLFW();
		void InitNFD();
		void InitWindow();
		void InitGLEW();
		void InitImGui();
		void InitCallbacks();
		void InitCursors();

		void UpdateFrame();
	}

	namespace update {

		namespace keyboard {
			void Keyboard();
			void Keyboard_Buttons_Callback(GLFWwindow* window, int key, int scancode, int action, int mods);
			extern std::vector<uint8_t> key_down;
			extern std::vector<uint8_t> key_pressed;

			inline bool isNotCorrectButton(const int& button);
		}

		namespace mouse {
			void Mouse();
			void Mouse_Buttons_Callback(GLFWwindow* window, int button, int action, int mods);
			void Mouse_Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset);
			extern double scroll_y;
			extern std::vector<uint8_t> mouse_down;
			extern std::vector<uint8_t> mouse_pressed;
		}

		namespace window {
			void Window();
			void Cursor_Enter_Callback(GLFWwindow* window, int entered);
			void Window_Size_Callback(GLFWwindow* window, int width, int height);
			void Framebuffer_Size_Callback(GLFWwindow* window, int width, int height);
		}

		namespace shader {
			void Shader();
			void MainBlock_Update();
		}
	}

	namespace input {
		bool IsKeyPressed(const KeyboardButton& button);
		bool IsKeyDown(const KeyboardButton& button);
		bool IsKeyUp(const KeyboardButton& button);

		bool IsMousePressed(const MouseButtons& button);
		bool IsMouseDown(const MouseButtons& button);
		bool IsMouseUp(const MouseButtons& button);

		const double& GetMouseScroll();

		glm::vec2 GetMouseWindow();
		glm::vec2 GetMouseScreen();
	}

	namespace time {
		float GetProgrammTime();
		float GetDeltaTime();

		void  SetTargetFPS(const int& value);
		void  LimitFrameRate();
	}

	namespace window {

		enum Cursor{
			Arrow,
			Hand,
			Drag,
			Text
		};

		enum WindowState {
			FULLSCREEN,
			WINDOWED
		};


		GLFWwindow* GetHandle();

		void SetCursor(const Cursor& type);


		void Centerize();
		void Maximize();
		void Hide();
		void Close();

		void SetTitle(const std::string& title);
		void SetPosition(const glm::vec2& pos);
		void SetSize(const glm::vec2& size);

		void SetFullscreen();
		void SetWindowed();
		void ToggleWindowState();
		const WindowState& GetWindowState();

		void EnableVSync();
		void DisableVSync();
		void ToggleVSync();
		const bool& GetVSyncState();

		glm::vec2			GetSize();
		const glm::vec2&	GetFramebufferSize();
		const float&		GetMinFramebufferSize();

		glm::vec2	GetPosition();
		std::string GetTitle();
		
		bool IsShouldClose();
	}

	namespace filedialog {
		struct FileDialogFilter {
			std::string name;
			std::string extensions; // "cpp,hpp,h"
		};

		std::optional<std::string> OpenSaveDialog(const std::string& nameWindow, const std::vector<FileDialogFilter>& filters);
		std::optional<std::string> OpenLoadDialog(const std::string& nameWindow, const std::vector<FileDialogFilter>& filters);
	}


	namespace shader{
		void BindUBO_MainBlock(const Shader& shader);
	}

}
#endif