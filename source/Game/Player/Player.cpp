#include "Player.h"

#include "Game/Math/Arkanoid_Math.h"

Player::Player() {
	size = { 0.15f, 0.02f };
	pos_left_bound = -1.f;
	pos_right_bound = 1.f;

	pos.y = 0.6f;
	pos.x = (pos_left_bound + pos_right_bound) / 2.f;
}

Player::~Player() {

}

void Player::Draw(QuadInstanced& renderer) {

	glm::vec2 size_screen = { TranslateScalar_GlobalToScreen(size.x), TranslateScalar_GlobalToScreen(size.y) };

	renderer.AddRectangle(pos, size_screen, glm::vec4(0.f,0.2,0.4f,1.f), TranslateGlobalToScreen);


	glm::vec2 p_s = pos;
	glm::vec2 n_s = perp_normalized({ 0.f,1.f }) * size.x;
	glm::vec2 t_s = glm::vec2{0.f,1.f} * size.y;

	const glm::vec2 v1 = p_s + n_s + t_s;
	const glm::vec2 v2 = p_s - n_s + t_s;
	const glm::vec2 v3 = p_s - n_s - t_s;
	const glm::vec2 v4 = p_s + n_s - t_s;


	renderer.AddLine(v1, v2, 2.f, glm::vec4(1.f), TranslateGlobalToScreen);
	renderer.AddLine(v2, v3, 2.f, glm::vec4(1.f), TranslateGlobalToScreen);
	renderer.AddLine(v3, v4, 2.f, glm::vec4(1.f), TranslateGlobalToScreen);
	renderer.AddLine(v4, v1, 2.f, glm::vec4(1.f), TranslateGlobalToScreen);

}

void Player::SetLeftBound(const float& global_value) {
	this->pos_left_bound = global_value;
}
void Player::SetRightBound(const float& global_value) {
	this->pos_right_bound = global_value;
}
void Player::SetBeginPosition(const glm::vec2& global_pos) {
	this->pos = global_pos;
	this->begin_position = pos;
}


void Player::Update() {
	pos.x = TranslateScreenToGlobal(engine::input::GetMouseWindow()).x;
	pos.x = glm::clamp(pos.x, pos_left_bound + size.x, pos_right_bound - size.x);
}

nlohmann::json Player::Save() {
	nlohmann::json data;
	
	data["begin_pos"]["x"] = begin_position.x;
	data["begin_pos"]["y"] = begin_position.y;

	data["size"]["x"] = size.x;
	data["size"]["y"] = size.y;

	data["left_bound"] = pos_left_bound;
	data["right_bound"] = pos_right_bound;


	return data;
}

void Player::Load(const nlohmann::json& data) {

	pos.x = data["begin_pos"]["x"].get<float>();
	pos.y = data["begin_pos"]["y"].get<float>();

	begin_position = pos;

	size.x = data["size"]["x"].get<float>();
	size.y = data["size"]["y"].get<float>();

	pos_left_bound = data["left_bound"].get<float>();
	pos_right_bound = data["right_bound"].get<float>();

}
