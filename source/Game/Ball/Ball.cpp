#include "Ball.h"

#include "Core/CoordinateSystem/CoordinateSystem.h"

void Ball::Draw(DebugCircle& renderer) {
	renderer.Add(path.begin, TranslateScalar_GlobalToScreen(radius), color, TranslateGlobalToScreen);
}
