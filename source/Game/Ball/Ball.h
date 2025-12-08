#ifndef BALL_H
#define BALL_H

#include <glm/glm.hpp>

struct Ball {
	glm::vec4 color;

	glm::vec2 prevPosition;
	glm::vec2 position;

	float speed;

	float radius_global;
};



#endif // !BALL_H

