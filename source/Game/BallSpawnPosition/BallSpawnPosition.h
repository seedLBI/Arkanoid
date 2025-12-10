#ifndef BALL_POSITION_SPAWN_H
#define BALL_POSITION_SPAWN_H

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>


struct BallSpawnPosition {
	float global_radius = 0.05f;
	glm::vec2 globalPos = {0.f,0.f};
	glm::vec2 tangent = {0.f,0.1f};


	nlohmann::json Save();
	void Load(const nlohmann::json& data);
};



#endif