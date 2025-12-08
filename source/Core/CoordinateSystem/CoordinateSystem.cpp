#include "CoordinateSystem.h"
#include "Core/Engine/Engine.h"
#include <algorithm>

glm::vec2 TranslateGlobalToScreen(const glm::vec2& global) {
	const glm::vec2& size = engine::window::GetFramebufferSize();
	const float& min = engine::window::GetMinFramebufferSize();

	return 	(size + global * min) / 2.f;
}

glm::vec2 TranslateScreenToGlobal(const glm::vec2& screen) {
	const glm::vec2& size = engine::window::GetFramebufferSize();
	const float& min = engine::window::GetMinFramebufferSize();

	return (screen * 2.f - size) / min;
}

float TranslateScalar_GlobalToScreen(const float& value) {
	return TranslateGlobalToScreen({ value,0.f }).x - TranslateGlobalToScreen({ 0.f,0.f }).x;
}

float TranslateScalar_ScreenToGlobal(const float& value) {
	return TranslateScreenToGlobal({ value,0.f }).x - TranslateScreenToGlobal({ 0.f,0.f }).x;
}


float ref_scale() {

	const glm::vec2& fSize = engine::window::GetFramebufferSize();
	float min = (std::min)(fSize.x, fSize.y);

	const glm::vec2 rSize = glm::vec2{ 1280.f, 720.f };

	glm::vec2 s{ min/rSize.x,min / rSize.y };

	return (std::min)(s.x, s.y);
}