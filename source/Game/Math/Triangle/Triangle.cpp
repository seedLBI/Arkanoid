#include "Triangle.h"

#include "Core/CoordinateSystem/CoordinateSystem.h"

void Triangle::DrawLine(QuadInstanced& renderer) {
	renderer.AddLine(p1, p2, glm::vec4(0.f, 1.f, 0.f, 1.f), TranslateGlobalToScreen);
	renderer.AddLine(p2, p3, glm::vec4(0.f, 1.f, 0.f, 1.f), TranslateGlobalToScreen);
	renderer.AddLine(p3, p1, glm::vec4(0.f, 1.f, 0.f, 1.f), TranslateGlobalToScreen);
}