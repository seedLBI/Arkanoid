#include "Triangle.h"

#include "Core/CoordinateSystem/CoordinateSystem.h"

void Triangle::DrawLine(QuadInstanced& renderer) {
	renderer.AddLine(p1, p2, glm::vec4(1.f, 0.2f, 1.f, 0.1f), TranslateGlobalToScreen);
	renderer.AddLine(p2, p3, glm::vec4(1.f, 0.2f, 1.f, 0.1f), TranslateGlobalToScreen);
	renderer.AddLine(p3, p1, glm::vec4(1.f, 0.2f, 1.f, 0.1f), TranslateGlobalToScreen);
}