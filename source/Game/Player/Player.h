#ifndef PLAYER_H
#define PLAYER_H

#include "Core/Engine/Engine.h"
#include "Core/CoordinateSystem/CoordinateSystem.h"

#include "Rendering/RenderObjects/QuadInstanced/Render.Quad.Instanced.h"

#include "Game/Math/AABB/AABB.h"

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

class Player {
public:
	Player();
	Player(const std::vector<glm::vec2>& mesh_player);
	~Player();

	void Draw(QuadInstanced& renderer);
	void DrawDebug(QuadInstanced& renderer);
	void Update();

	void SetLeftBound(const float& global_value);
	void SetRightBound(const float& global_value);
	void SetBeginPosition(const glm::vec2& global_pos);


	void UpdateRadius(const float& global_radius);

	const AABB_Region& GetCurrentAABB();
	const std::vector<glm::vec2>& GetVertices();
	const std::vector<glm::vec2>& GetVertices_OriginalBorder();

	const glm::vec2& GetBeginPosition();
	const float& GetLeftBound();
	const float& GetRightBound();

	nlohmann::json Save();
	void Load(const nlohmann::json& data);

private:
	std::vector<glm::vec2> mesh;
	std::vector<glm::vec2> mesh_border;


	std::vector<glm::vec2> updated_mesh;
	std::vector<glm::vec2> updated_mesh_border;
	AABB_Region updated_aabb;

	glm::vec2 begin_position;

	glm::vec2 pos;
	AABB_Region aabb;

	float pos_left_bound;
	float pos_right_bound;

};


#endif