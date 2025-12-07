#ifndef PLAYER_H
#define PLAYER_H

#include "Core/Engine/Engine.h"
#include "Core/CoordinateSystem/CoordinateSystem.h"

#include "Rendering/RenderObjects/DebugLine/Render.Debug.Line.h"

#include <glm/glm.hpp>

class Player {
public:
	Player();
	~Player();

	void Draw(DebugLine& renderer);
	void Update();

private:
	glm::vec2 pos;
	glm::vec2 size;

	float pos_left_bound;
	float pos_right_bound;

};

Player::Player() {

}

Player::~Player() {

}

void Player::Draw(DebugLine& renderer) {

}

void Player::Update() {

}

#endif