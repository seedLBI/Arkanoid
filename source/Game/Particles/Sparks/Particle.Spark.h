#ifndef PARTICLES_SPARK_H
#define PARTICLES_SPARK_H

#include <glm/glm.hpp>

struct ParticleSpark {
	glm::vec2 pos;
	glm::vec2 dir;
	float speed;
	float size;

	float timeCrated;
	float timeLife;

	glm::vec4 color;
};



#endif