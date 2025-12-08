#ifndef PLAYER_H
#define PLAYER_H

#include "Core/Engine/Engine.h"
#include "Core/CoordinateSystem/CoordinateSystem.h"

#include "Rendering/RenderObjects/QuadInstanced/Render.Quad.Instanced.h"

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

class Player {
public:
	Player();
	~Player();

	void Draw(QuadInstanced& renderer);
	void Update();

	void SetLeftBound(const float& global_value);
	void SetRightBound(const float& global_value);
	void SetBeginPosition(const glm::vec2& global_pos);


	const glm::vec2& GetBeginPosition();
	const float& GetLeftBound();
	const float& GetRightBound();

	nlohmann::json Save();
	void Load(const nlohmann::json& data);

private:

	glm::vec2 begin_position;

	glm::vec2 pos;
	glm::vec2 size;

	float pos_left_bound;
	float pos_right_bound;

};


#endif