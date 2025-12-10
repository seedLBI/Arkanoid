#ifndef LELEL_CREATOR_H
#define LELEL_CREATOR_H

#include <imgui.h>
#include <nlohmann/json.hpp>

#include "Game/CollisionInfo/CollisionInfo.h"
#include "Game/DestroyableObject/DestroyableObject.h"
#include "Game/LevelBorder/LevelBorder.h"
#include "Game/Player/Player.h"

#include "Rendering/RenderObjects/QuadInstanced/Render.Quad.Instanced.h"
#include "Rendering/RenderObjects/DebugCircle/Render.Debug.Circle.h"

class LevelCreator {
public:
	LevelCreator();
	~LevelCreator();

	void Draw(DebugCircle& circles, QuadInstanced& quads);
	void Update();

	void Save();
	void Load();

private:


	// Border
	bool flag_MODE_CreatorBorder = true;
	LevelBorder levelBorder;
	std::vector<glm::vec2> vertices_border;
	float global_radius = 0.05f;

	int index_vertex_cover = -1;
	int index_vertex_grab = -1;
	float radius_control_points = 15.f;


	// Player
	bool flag_MODE_EditPlayer = false;
	Player player;
	bool player_hovered = false, player_draging = false;
	float radius_control_player_position = 30.f;
	glm::vec2 global_pos_player = {0.f,0.f};
	float global_left_border_player = -1.f;
	float global_right_border_player = 1.f;

	// DestroyableObject
	bool flag_MODE_CreatorDestroyableObject = false;
	std::vector<DestroyableObject> destroyable;
	std::vector<std::vector<glm::vec2>> mesh_destroyables;
	std::vector < glm::vec2> mesh_movable;

	std::vector<int> choosedForEdit;

	int index_destroyable_choosed = -1;

	int index_destroyable_cover_vertex = -1;
	int index_destroyable_grab_vertex = -1;

	int index_destroyable_cover = -1;
	int index_destroyable_grab = -1;


};




#endif
