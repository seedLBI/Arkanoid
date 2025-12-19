#ifndef PARTICLE_TEXT_H
#define PARTICLE_TEXT_H

#include <glm/glm.hpp>
#include "Core/Text/Text.Atlas.h"

struct ParticleText {
	glm::vec2 pos;
	glm::vec2 direction;
	float speed = 1.f;

	float size_global = 0.1f;

	float timeCreated = 0.f;
	float timeLife    = 1.f;

	TextAtlas text;
};

#endif