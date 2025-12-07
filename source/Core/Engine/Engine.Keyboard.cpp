#include "Engine.h"
#include "Engine.Keyboard.Enums.h"

namespace engine {

    namespace update {
        namespace keyboard {
            std::vector<uint8_t> key_down(KEY_LAST, 0);
            std::vector<uint8_t> key_pressed(KEY_LAST, 0);

            void Keyboard() {
                std::fill(key_pressed.begin(), key_pressed.end(), 0);
            }

            void Keyboard_Buttons_Callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
                ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

                if (isNotCorrectButton(key)) return;

                if (action == GLFW_PRESS) {
                    key_down[key] = 1;
                    key_pressed[key] = 1; // активен только в этом кадре
                }
                else if (action == GLFW_RELEASE) {
                    key_down[key] = 0;
                }
            }

            inline bool isNotCorrectButton(const int& button) {
                return button < 0 || button > GLFW_KEY_LAST;
            }
        }
    }

    namespace input {

        bool IsKeyPressed(const KeyboardButton& button) {
            if (update::keyboard::isNotCorrectButton(button)) return false;

            using namespace update::keyboard;

            int state = key_pressed[button];
            key_pressed[button] = false;

            return state;
        }


        bool IsKeyDown(const KeyboardButton& button) {
            if (update::keyboard::isNotCorrectButton(button)) return false;

            using namespace update::keyboard;

            return key_down[button];
        }

        bool IsKeyUp(const KeyboardButton& button) {
            if (update::keyboard::isNotCorrectButton(button)) return false;

            using namespace update::keyboard;

            return key_down[button] == false;
        }
    }
}