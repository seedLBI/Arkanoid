#include "Player.h"

#include "Game/Math/Arkanoid_Math.h"


Player::Player() {

	glm::vec2 size = { 0.15f,0.02f };
	this->mesh = { -size, {-size.x,size.y}, size, {size.x,-size.y} };
	this->mesh.push_back(this->mesh.front());

	this->mesh_border = GenerateRadiusBorder(this->mesh, 0.05f, true);
	this->aabb = GetAABB(this->mesh_border);

	updated_mesh_border = mesh_border;

	pos_left_bound = -1.f;
	pos_right_bound = 1.f;

	pos.y = 0.6f;
	pos.x = (pos_left_bound + pos_right_bound) / 2.f;

	begin_position = pos;
}

Player::Player(const std::vector<glm::vec2>& mesh_player) {

	this->mesh = mesh_player;
	this->mesh.push_back(this->mesh.front());

	this->mesh_border = GenerateRadiusBorder(this->mesh, 0.05f, true);
	this->aabb = GetAABB(this->mesh_border);

	updated_mesh_border = mesh_border;

	pos_left_bound = -1.f;
	pos_right_bound = 1.f;

	pos.y = 0.6f;
	pos.x = (pos_left_bound + pos_right_bound) / 2.f;

	begin_position = pos;
}

Player::~Player() {

}

void Player::Draw(QuadInstanced& renderer) {
	for (size_t i = 0; i < mesh.size() - 1; i++)
		renderer.AddLine(mesh[i] + pos, mesh[i + 1] + pos,2.f, glm::vec4(1.f), TranslateGlobalToScreen);
}

void Player::DrawDebug(QuadInstanced& renderer) {
	for (size_t i = 0; i < updated_mesh_border.size() - 1; i++)
		renderer.AddLine(updated_mesh_border[i], updated_mesh_border[i + 1],1.f, glm::vec4(0.f, 1.f, 0.f, 1.f), TranslateGlobalToScreen);

	//renderer.AddRectangleLines(updated_aabb.min, updated_aabb.max, 2.f, glm::vec4(0.32f, 0.43f, 1.f, 1.f), TranslateGlobalToScreen);
}

const AABB_Region& Player::GetCurrentAABB() {
	return updated_aabb;
}
const std::vector<glm::vec2>& Player::GetVertices() {
	return updated_mesh_border;
}
const std::vector<glm::vec2>& Player::GetVertices_OriginalBorder() {
	return updated_mesh;
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

const glm::vec2& Player::GetBeginPosition() {
	return begin_position;
}
const float& Player::GetLeftBound() {
	return pos_left_bound;
}
const float& Player::GetRightBound() {
	return pos_right_bound;
}


void Player::Update() {
	pos.x = TranslateScreenToGlobal(engine::input::GetMouseWindow()).x;

	glm::vec2 pos_center = pos + (aabb.min + aabb.max) / 2.f;
	float width = (aabb.max.x - aabb.min.x);

	pos.x = glm::clamp(pos_center.x, pos_left_bound  + width/2.f, pos_right_bound - width/2.f);


	for (size_t i = 0; i < mesh_border.size(); i++)
		updated_mesh_border[i] = mesh_border[i] + pos;

	for (size_t i = 0; i < mesh.size(); i++)
		updated_mesh[i] = mesh[i] + pos;


	updated_aabb = aabb;

	updated_aabb.min += pos;
	updated_aabb.max += pos;
}


void Player::UpdateRadius(const float& global_radius) {

	mesh_border = GenerateRadiusBorder(this->mesh, global_radius, true);
	this->aabb = GetAABB(mesh_border);

	updated_mesh_border.resize(mesh_border.size());
	updated_mesh.resize(mesh.size());

}


nlohmann::json Player::Save() {
	nlohmann::json data;
	
	data["begin_pos"]["x"] = begin_position.x;
	data["begin_pos"]["y"] = begin_position.y;

	for (size_t i = 0; i < mesh.size(); i++) {
		data["mesh"][i]["x"] = mesh[i].x;
		data["mesh"][i]["y"] = mesh[i].y;
	}

	data["left_bound"] = pos_left_bound;
	data["right_bound"] = pos_right_bound;


	return data;
}
void Player::Load(const nlohmann::json& data) {

	pos.x = data["begin_pos"]["x"].get<float>();
	pos.y = data["begin_pos"]["y"].get<float>();

	begin_position = pos;

	mesh.clear();

	if (data.contains("mesh"))
	{
		for (const auto& [key, meshData] : data["mesh"].items()) {

			glm::vec2 temp;

			temp.x = meshData["x"].get<float>();
			temp.y = meshData["y"].get<float>();

			mesh.push_back(temp);
		}

		mesh_border = GenerateRadiusBorder(mesh, 0.05f, true);
		aabb = GetAABB(mesh_border);
	}
	else {
		glm::vec2 size = { 0.15f,0.02f };
		this->mesh = { -size, {-size.x,size.y}, size, {size.x,-size.y} };
		this->mesh.push_back(this->mesh.front());
		mesh_border = GenerateRadiusBorder(mesh, 0.05f, true);
		aabb = GetAABB(mesh_border);
	}

	updated_mesh_border = mesh_border;
	updated_mesh = mesh;


	pos_left_bound = data["left_bound"].get<float>();
	pos_right_bound = data["right_bound"].get<float>();

}
