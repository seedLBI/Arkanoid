#include "Triangulation.Greedy.h"
#include "Game/Math/Arkanoid_Math.h"
#include "Game/Math/Segment/Segment.h"

std::vector<Triangle> MakeTriangulationGreedy(const std::vector<glm::vec2> polygon) {
	std::vector<Triangle> trigs;

	for (size_t i = 0; i < polygon.size(); i++) {

		const glm::vec2& i_p = polygon[i];

		size_t index_min;
		float len;
		
		for (size_t j = 0; j < polygon.size(); j++) {
			if (i == j) continue;

			const glm::vec2& j_p = polygon[i];

			float cur_len = glm::length(i_p - j_p);
			


			
		}

	}


	return trigs;
}
