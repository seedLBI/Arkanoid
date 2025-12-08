#include "LevelCreator.h"
#include "Game/Math/Arkanoid_Math.h"
#include "Core/CoordinateSystem/CoordinateSystem.h"

#include <nfd_glfw3.h>
#include <fstream>

LevelCreator::LevelCreator() {
	global_pos_player = player.GetBeginPosition();
}

LevelCreator::~LevelCreator() {

}

void LevelCreator::Draw(DebugCircle& circles, QuadInstanced& quads) {
	if (flag_MODE_CreatorBorder) {
		quads.AddLine(glm::vec2(-1.f, -1.f), glm::vec2(-1.f, 1.f), glm::vec4(1.f), TranslateGlobalToScreen);
		quads.AddLine(glm::vec2(-1.f, 1.f), glm::vec2(1.f, 1.f), glm::vec4(1.f), TranslateGlobalToScreen);
		quads.AddLine(glm::vec2(1.f, 1.f), glm::vec2(1.f, -1.f), glm::vec4(1.f), TranslateGlobalToScreen);
		quads.AddLine(glm::vec2(1.f, -1.f), glm::vec2(1.f, 1.f), glm::vec4(1.f), TranslateGlobalToScreen);

		levelBorder.DrawDebug(quads);

		for (size_t i = 0; i < vertices_border.size(); i++)
		{
			glm::vec4 color(1.f, 0.3f, 0.3f, 1.f);
			if (index_vertex_cover == i)
				color.r = 0.1f;
			circles.Add(vertices_border[i], radius_control_points, color, TranslateGlobalToScreen);
		}


	}

	if (flag_MODE_EditPlayer)
	{
		circles.Add(global_pos_player, radius_control_player_position, glm::vec4(0.3f, 0.3f, 1.f, 0.8f), TranslateGlobalToScreen);

		quads.AddLine({ global_left_border_player,-1.f }, { global_left_border_player,1.f }, 10.f, glm::vec4(1.f, 0.f, 0.f, 0.5f), TranslateGlobalToScreen);
		quads.AddLine({ global_right_border_player,-1.f }, { global_right_border_player,1.f }, 10.f, glm::vec4(1.f, 0.f, 0.f, 0.5f), TranslateGlobalToScreen);

		quads.AddLine({ -10.f, global_pos_player.y }, { 10.f, global_pos_player.y }, 3.f, glm::vec4(1.f, 0.4f, 0.4f, 0.8f), TranslateGlobalToScreen);
	}


	levelBorder.Draw(quads);
	player.Draw(quads);



	ImGui::Begin("Level Creator");

	if (ImGui::Button("Save")) {
		Save();
	}
	ImGui::SameLine();
	if (ImGui::Button("Load")) {
		Load();
	}



	if (ImGui::RadioButton("BorderMode", flag_MODE_CreatorBorder)) {
		flag_MODE_CreatorBorder = !flag_MODE_CreatorBorder;
		flag_MODE_EditPlayer = false;
	}

	if (ImGui::RadioButton("PlayerEditorMode", flag_MODE_EditPlayer)) {
		flag_MODE_EditPlayer = !flag_MODE_EditPlayer;
		flag_MODE_CreatorBorder = false;
	}

	if (flag_MODE_EditPlayer)
	{
		if (ImGui::SliderFloat("leftBound", &global_left_border_player, -2.f, 2.f)) {
			player.SetLeftBound(global_left_border_player);
		}
		if (ImGui::SliderFloat("rightBound", &global_right_border_player, -2.f, 2.f)) {
			player.SetRightBound(global_right_border_player);
		}
	}




	ImGui::End();
}
void LevelCreator::Update() {
	
	player.Update();

	glm::vec2 mouse_global = TranslateScreenToGlobal(engine::input::GetMouseWindow());

	if (flag_MODE_CreatorBorder) {

		if (engine::input::IsMousePressed(MOUSE_RIGHT)) {
			vertices_border.push_back(mouse_global);
			levelBorder.SetVertices(vertices_border, global_radius);
			
		}

		index_vertex_cover = -1;
		for (size_t i = 0; i < vertices_border.size(); i++) {

			if (isIntersectPointCircle(mouse_global, vertices_border[i], TranslateScalar_ScreenToGlobal(radius_control_points))){

				index_vertex_cover = i;

				if (engine::input::IsMousePressed(MOUSE_LEFT)) {
					index_vertex_grab = i;
					break;
				}

				if (engine::input::IsKeyPressed(KEY_X)) {
					vertices_border.erase(vertices_border.begin() + i);
					break;
				}
			}

		}

		if (engine::input::IsMouseDown(MOUSE_LEFT) && index_vertex_grab != -1) {
			vertices_border[index_vertex_grab] = mouse_global;

			levelBorder.SetVertices(vertices_border, global_radius);
		}
		else {
			index_vertex_grab = -1;
		}
		


	}
	else if (flag_MODE_EditPlayer) {


		player_hovered = false;
		
		if (isIntersectPointCircle(mouse_global, global_pos_player, TranslateScalar_ScreenToGlobal(radius_control_player_position))) {
			player_hovered = true;

			if (engine::input::IsMousePressed(MOUSE_LEFT)) {
				player_draging = true;
			}
		}


		if (engine::input::IsMouseDown(MOUSE_LEFT) && player_draging) {
			global_pos_player = mouse_global;
			player.SetBeginPosition(global_pos_player);
		}
		else
			player_draging = false;


	}


}


void LevelCreator::Save() {
	engine::filedialog::FileDialogFilter filter;
	filter.name = "Level Arkanoid";
	filter.extensions = "level";

	auto path = engine::filedialog::OpenSaveDialog("Save level", { filter });
	if (path.has_value()) {

		nlohmann::json data;

		data["border"] = levelBorder.Save();
		data["player"] = player.Save();

		std::ofstream ofn(path.value());

		ofn << data.dump(4);

		ofn.close();
	}
}
void LevelCreator::Load() {
	engine::filedialog::FileDialogFilter filter;
	filter.name = "Level Arkanoid";
	filter.extensions = "level";

	auto path = engine::filedialog::OpenLoadDialog("Load level", { filter });
	if (path.has_value()) {
		std::ifstream ifn(path.value());

		nlohmann::json data = nlohmann::json::parse(ifn);
		ifn.close();

		if (data.contains("border")) {
			levelBorder.Load(data["border"]);

			vertices_border.clear();

			const auto& vertices_level = levelBorder.GetVertices_OriginalBorder();
			vertices_border.resize(vertices_level.size() - 1);

			for (size_t i = 0; i < vertices_level.size() - 1; i++)
				vertices_border[i] = vertices_level[i];


		}

		if (data.contains("player")) {
			player.Load(data["player"]);

			global_left_border_player = player.GetLeftBound();
			global_right_border_player = player.GetRightBound();

			global_pos_player = player.GetBeginPosition();


		}



	}
}