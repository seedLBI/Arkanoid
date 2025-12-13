#ifndef TRINGULATION_GREEDY_H
#define TRINGULATION_GREEDY_H

#include <glm/glm.hpp>
#include <vector>


struct Triangle {
	glm::vec2 p1, p2, p3;
};


std::vector<Triangle> MakeTriangulationGreedy(
	const std::vector<glm::vec2> polygon);





#endif