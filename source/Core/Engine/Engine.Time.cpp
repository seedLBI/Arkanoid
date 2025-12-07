#include "Engine.h"

namespace engine {
	namespace time {

		float GetProgrammTime() {
			return glfwGetTime();
		}

		float GetDeltaTime() {
			return engine::core::vars::fps_limiter->GetDeltaTime();
		}

		void SetTargetFPS(const int& value) {
			engine::core::vars::fps_limiter->SetTargetFPS(value);
		}

		void LimitFrameRate() {
			engine::core::vars::fps_limiter->wait();
		}
	}
}