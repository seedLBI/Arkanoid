#include "Engine.h"
#include "Engine.Mouse.Enums.h"

namespace engine {
    namespace update {
        namespace mouse {

            std::vector<uint8_t> mouse_down(3, 0);
            std::vector<uint8_t> mouse_pressed(3, 0);

            double scroll_y = 0;

            void Mouse() {
                std::fill(mouse_pressed.begin(), mouse_pressed.end(), 0);
                scroll_y = 0;
            }

            void Mouse_Buttons_Callback(GLFWwindow* window, int button, int action, int mods) {
                ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

                if (button < 0 || button > 3) return;

                if (action == GLFW_PRESS) {
                    mouse_down[button] = 1;
                    mouse_pressed[button] = 1;
                }
                else if (action == GLFW_RELEASE) {
                    mouse_down[button] = 0;
                }
            }

            void Mouse_Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset) {
                scroll_y = yoffset;
            }
        }
    }

    namespace input {
        bool IsMouseDown(const MouseButtons& button) {
            if (button < 0 || button > 3) return false;
            using namespace update::mouse;
            return mouse_down[button];
        }

        bool IsMouseUp(const MouseButtons& button) {
            if (button < 0 || button > 3) return false;
            using namespace update::mouse;
            return mouse_down[button] == false;
        }

        bool IsMousePressed(const MouseButtons& button) {
            if (button < 0 || button > 3) return false;
            using namespace update::mouse;
            int state = mouse_pressed[button];
            mouse_pressed[button] = 0;
            return state;
        }

        glm::vec2 GetMouseWindow() {
            double x, y;
            glfwGetCursorPos(core::vars::handle_window, &x, &y);
            return { x,y };
        }

        glm::vec2 GetMouseScreen() {
            double x, y;
            glfwGetCursorPos(core::vars::handle_window, &x, &y);

            int winX, winY;
            glfwGetWindowPos(core::vars::handle_window, &winX, &winY);

            return  GetMouseWindow() + glm::vec2{winX, winY};
        }

        const double& GetMouseScroll() {
            return update::mouse::scroll_y;
        }
    }
}