#include "Ball.h"

#include "Core/CoordinateSystem/CoordinateSystem.h"

void Ball::Draw(DebugCircle& renderer) {
	renderer.Add(path.begin, TranslateScalar_GlobalToScreen(radius*1.2f), glm::vec4(0.f,0.f,0.f,1.f), TranslateGlobalToScreen);

	renderer.Add(path.begin, TranslateScalar_GlobalToScreen(radius), color, TranslateGlobalToScreen);
}

void Ball::DrawDebug(DebugCircle& renderer, QuadInstanced& quads) {
	renderer.Add(path.end, TranslateScalar_GlobalToScreen(radius), glm::vec4(1.f,0.f,0.f,0.5f), TranslateGlobalToScreen);

	quads.AddLine(path.end, path.end + tangent, 2.f, glm::vec4(0.f, 1.f, 0.f, 1.f), TranslateGlobalToScreen);
}