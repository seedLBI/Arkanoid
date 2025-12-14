#include "Triangle.h"

#include "Core/CoordinateSystem/CoordinateSystem.h"

#include "Game/Math/Arkanoid_Math.h"

void Triangle::DrawLine(QuadInstanced& renderer) {
	renderer.AddLine(p1, p2, glm::vec4(1.f, 0.2f, 1.f, 0.1f), TranslateGlobalToScreen);
	renderer.AddLine(p2, p3, glm::vec4(1.f, 0.2f, 1.f, 0.1f), TranslateGlobalToScreen);
	renderer.AddLine(p3, p1, glm::vec4(1.f, 0.2f, 1.f, 0.1f), TranslateGlobalToScreen);

}

float Triangle::getArea() const {
	return getAreaTriangle(p1, p2, p3);
}


bool Collision_Point_and_Triangle(const glm::vec2& point, const Triangle& triangle) {

	float area = triangle.getArea();

	if (area == 0.f) return false;

	area = 1.f / area;

	//barycentric
	float u = getAreaTriangle(triangle.p2, triangle.p3, point) * area;
	float v = getAreaTriangle(triangle.p3, triangle.p1, point) * area;
	float w = getAreaTriangle(triangle.p1, triangle.p2, point) * area;

	bool inTriangle = u >= 0.f && v >= 0.f && w >= 0.f;

	return inTriangle;
}