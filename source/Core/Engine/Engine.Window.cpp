#include "Engine.h"

namespace engine {

	namespace update{
		namespace window{
			void Window(){
				
			}
			void Cursor_Enter_Callback(GLFWwindow* window, int entered){

				if (entered) {
					//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				} else {
					//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				}
			}

			void Window_Size_Callback(GLFWwindow* window, int width, int height){

			}

			void Framebuffer_Size_Callback(GLFWwindow* window, int width, int height){
				engine::core::vars::sizeFramebuffer = {width,height};
				engine::core::vars::minSizeFramebuffer = (std::min)(width, height);

				glViewport(0, 0, width, height);
			}

		}
	}

	namespace window {
		using namespace engine::core::vars;

		void SetCursor(const Cursor& type){
			currentCursor = (int)type;
			glfwSetCursor(handle_window, cursors[currentCursor]);

		}

		const glm::vec2& GetFramebufferSize(){
			return sizeFramebuffer;
		}
		const float& GetMinFramebufferSize() {
			return minSizeFramebuffer;
		}


		GLFWwindow* GetHandle() {
			return handle_window;
		}

		void Centerize() {
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);

			int currentWidth, currentHeight;
			glfwGetWindowSize(handle_window, &currentWidth, &currentHeight);
			glfwSetWindowPos(handle_window, (mode->width - currentWidth) / 2, (mode->height - currentHeight) / 2);
		}
		void Maximize() {
			glfwMaximizeWindow(handle_window);
		}
		void Hide() {
			glfwHideWindow(handle_window);
		}
		void Close() {
			glfwSetWindowShouldClose(handle_window, 1);
		}

		void SetTitle(const std::string& title) {
			glfwSetWindowTitle(handle_window, title.c_str());
		}
		void SetPosition(const glm::vec2& pos) {
			glfwSetWindowPos(handle_window, static_cast<int>(pos.x), static_cast<int>(pos.y));
		}
		void SetSize(const glm::vec2& size) {

			sizeFramebuffer = { size.x,size.y };
			minSizeFramebuffer = (std::min)(size.x, size.y);

			glfwSetWindowSize(handle_window, static_cast<int>(size.x), static_cast<int>(size.y));
			glViewport(0, 0, size.x, size.y);
		}

		void SetFullscreen() {
			if (window_state == WindowState::FULLSCREEN)
				return;

			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);

			if (glfwGetWindowMonitor(handle_window) == nullptr) {
				last_pos_window = GetPosition();
				last_size_window = GetSize();
			}

			glfwSetWindowMonitor(handle_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

			sizeFramebuffer = glm::vec2(static_cast<float>(mode->width), static_cast<float>(mode->height));
			minSizeFramebuffer = (std::min)(static_cast<float>(mode->width), static_cast<float>(mode->height));
			glViewport(0, 0, mode->width, mode->height);

			window_state = WindowState::FULLSCREEN;

		}
		void SetWindowed() {

			if (glfwGetWindowMonitor(handle_window) == nullptr)
				return;

			int width = (last_size_window.x > 0) ? last_size_window.x : 1280;
			int height = (last_size_window.y > 0) ? last_size_window.y : 720;
			int pos_x = (last_pos_window.x >= 0) ? last_pos_window.x : 100;
			int pos_y = (last_pos_window.y >= 0) ? last_pos_window.y : 100;

			glfwSetWindowMonitor(handle_window, nullptr, pos_x, pos_y, width, height, GLFW_DONT_CARE);

			sizeFramebuffer = glm::vec2(static_cast<float>(width), static_cast<float>(height));
			minSizeFramebuffer = (std::min)(static_cast<float>(width), static_cast<float>(height));
			glViewport(0, 0, width, height);

			window_state = WindowState::WINDOWED;
		}
		void ToggleWindowState() {
			if (window_state == WindowState::WINDOWED)
				SetFullscreen();
			else
				SetWindowed();
		}
		const WindowState& GetWindowState() {
			return (WindowState)window_state;
		}

		void EnableVSync() {
			glfwSwapInterval(1);
			VSync_state = 1;
		}
		void DisableVSync() {
			glfwSwapInterval(0);
			VSync_state = 0;
		}
		void ToggleVSync() {
			if (VSync_state)
				DisableVSync();
			else
				EnableVSync();
		}
		const bool& GetVSyncState() {
			return VSync_state;
		}

		glm::vec2 GetSize() {
			int width, height;
			glfwGetWindowSize(handle_window, &width, &height);
			return { width,height };
		}
		glm::vec2	GetPosition() {
			int posX = 0, posY = 0;
			glfwGetWindowPos(handle_window, &posX, &posY);
			return glm::vec2(posX, posY);
		}
		std::string GetTitle() {
			return glfwGetWindowTitle(handle_window);
		}



		bool IsShouldClose() {
			return glfwWindowShouldClose(handle_window);
		}

	}
}
