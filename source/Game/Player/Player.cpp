#include "Player.h"

#include "Game/Math/Arkanoid_Math.h"


Player::Player() {

	glm::vec2 size = { 0.15f,0.02f };
	this->mesh = { -size, {-size.x,size.y}, size, {size.x,-size.y} };

	if (isClockwise(mesh, true) == false) {
		std::reverse(mesh.begin(), mesh.end());
	}

	this->mesh_border = GenerateRadiusBorder(this->mesh, 0.05f, true);
	this->aabb = GetAABB(this->mesh_border);

	updated_mesh_border = mesh_border;
	updated_mesh = mesh;

	pos_left_bound = -1.f;
	pos_right_bound = 1.f;

	pos.y = 0.6f;
	pos.x = (pos_left_bound + pos_right_bound) / 2.f;

	begin_position = pos;
}

Player::Player(const std::vector<glm::vec2>& mesh_player) {

	this->mesh = mesh_player;
	if (isClockwise(mesh, true) == false) {
		std::reverse(mesh.begin(), mesh.end());
	}
	this->mesh_border = GenerateRadiusBorder(this->mesh, 0.05f, true);
	this->aabb = GetAABB(this->mesh_border);

	updated_mesh_border = mesh_border;
	updated_mesh = mesh;

	pos_left_bound = -1.f;
	pos_right_bound = 1.f;

	pos.y = 0.6f;
	pos.x = (pos_left_bound + pos_right_bound) / 2.f;

	begin_position = pos;
}

Player::~Player() {

}

void Player::Draw(QuadInstanced& renderer) {
	for (size_t i = 0; i < mesh.size(); i++)
		renderer.AddLine(mesh[i] + pos + glm::vec2{0.f, fake_height_anim}, mesh[(i + 1) % mesh.size()] + pos + glm::vec2{0.f, fake_height_anim}, 2.f, glm::vec4(1.f), TranslateGlobalToScreen);
}

void Player::DrawDebug(QuadInstanced& renderer) {
	for (size_t i = 0; i < updated_mesh_border.size(); i++) {

		glm::vec2& begin = updated_mesh_border[i];
		glm::vec2& end = updated_mesh_border[(i + 1) % updated_mesh_border.size()];

		renderer.AddLine(begin, end, 1.f, glm::vec4(0.f, 1.f, 0.f, 1.f), TranslateGlobalToScreen);

		glm::vec2 perp = perp_normalized(getDirection(begin, end)) * 0.05f;
		const glm::vec4 color_normal = glm::vec4{ 1.f,0.f,1.f,1.f };

		renderer.AddLine(begin, begin + perp, 1.f, color_normal, TranslateGlobalToScreen);
		renderer.AddLine(end, end + perp, 1.f,     color_normal, TranslateGlobalToScreen);

	}


	renderer.AddRectangleLines(updated_aabb.min, updated_aabb.max, 1.f, glm::vec4(0.32f, 0.43f, 1.f, 0.4f), TranslateGlobalToScreen);
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


void Player::ReactToCollision() {
	fake_height_anim = fake_height_anim_value;
}


glm::vec2 Player::GetDirectionMove() {
	if (pos == last_pos)
		return { 0.f,0.f };
	else
		return glm::normalize(pos - last_pos);
}


void Player::Update() {
	fake_height_anim = fake_height_anim + engine::time::GetDeltaTime() * 4.f * (0.f - fake_height_anim);


	last_pos = pos;
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

	mesh_border = GenerateRadiusBorder(this->fake_mesh, global_radius, true);
	this->aabb = GetAABB(mesh_border);

	updated_mesh_border.resize(mesh_border.size());
	updated_mesh.resize(mesh.size());

}

float Player::GetHeight() {
	return updated_aabb.max.y;
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
		int sampleRate = 4;
		for (size_t i = 0; i <= sampleRate; i++)
		{
			float angle = -(glm::pi<float>() / (float)sampleRate) * (float)i;

			glm::vec2 p = getPointOnCircle(angle) * 0.1f;

			p.x *= 2.f;

			mesh.push_back(p);

		}

		fake_mesh = mesh;

		fake_mesh.insert(fake_mesh.begin(), { fake_mesh.front().x,1.f });
		fake_mesh.insert(fake_mesh.end(),   { fake_mesh.back().x,1.f});


		if (mesh.front() == mesh.back())
			mesh.pop_back();

		if (isClockwise(mesh, true) == false) {
			std::reverse(mesh.begin(), mesh.end());
		}


		/*
		for (const auto& [key, meshData] : data["mesh"].items()) {

			glm::vec2 temp;

			temp.x = meshData["x"].get<float>();
			temp.y = meshData["y"].get<float>();

			mesh.push_back(temp);
		}
		*/




		mesh_border = GenerateRadiusBorder(fake_mesh, 0.05f, isClockwise(fake_mesh, true));
		aabb = GetAABB(mesh_border);
	}
	else {
		glm::vec2 size = { 0.15f,0.02f };
		this->mesh = { -size, {-size.x,size.y}, size, {size.x,-size.y} };
		mesh_border = GenerateRadiusBorder(mesh, 0.05f, true);
		aabb = GetAABB(mesh_border);
	}

	updated_mesh_border = mesh_border;
	updated_mesh = mesh;

	printf("\n\nPLAYER\n[");
	for (size_t i = 0; i < mesh.size(); i++) {

		printf("(%f, %f)", mesh[i].x, mesh[i].y);
		if (i != mesh.size() - 1)
			printf(",");
	}
	printf("]\n");

	printf("\n\nPLAYER_BORDER\n[");
	for (size_t i = 0; i < mesh_border.size(); i++) {

		printf("(%f, %f)", mesh_border[i].x, mesh_border[i].y);
		if (i != mesh_border.size() - 1)
			printf(",");
	}
	printf("]\n");


	pos_left_bound  = data["left_bound"].get<float>();
	pos_right_bound = data["right_bound"].get<float>();

}
