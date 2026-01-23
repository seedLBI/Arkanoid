#include "LevelCreator.h"
#include "Game/Math/Arkanoid_Math.h"
#include "Core/CoordinateSystem/CoordinateSystem.h"

#include <nfd_glfw3.h>
#include <fstream>

LevelCreator::LevelCreator() {
	global_pos_player = player.GetBeginPosition();
	choosedForEdit.reserve(100);
}

LevelCreator::~LevelCreator() {

}

void LevelCreator::Draw(DebugCircle& circles, QuadInstanced& quads, TriangleInstanced& triangles, TextInstanced& text_renderer, FontAtlas& font) {
	if (flag_MODE_CreatorBorder) {
		quads.AddLine(glm::vec2(-1.f, -1.f), glm::vec2(-1.f,  1.f), glm::vec4(1.f), TranslateGlobalToScreen);
		quads.AddLine(glm::vec2(-1.f,  1.f), glm::vec2( 1.f,  1.f), glm::vec4(1.f),   TranslateGlobalToScreen);
		quads.AddLine(glm::vec2( 1.f,  1.f), glm::vec2( 1.f, -1.f), glm::vec4(1.f),   TranslateGlobalToScreen);
		quads.AddLine(glm::vec2( 1.f, -1.f), glm::vec2( 1.f,  1.f), glm::vec4(1.f),   TranslateGlobalToScreen);


		levelBorder.DrawDebug(quads, false);

		for (size_t i = 0; i < vertices_border.size(); i++)
		{
			glm::vec4 color(1.f, 0.3f, 0.3f, 1.f);
			if (index_vertex_cover == i)
				color.r = 0.1f;
			circles.Add(vertices_border[i], radius_control_points, color, TranslateGlobalToScreen);


			makeText(std::to_string(i), glm::vec4(1.f,0.f,0.f,1.f)).addToRender(text_renderer, font, TranslateGlobalToScreen(vertices_border[i]), 50.f, 0.f);




		}


	}

	if (flag_MODE_EditPlayer)
	{
		circles.Add(global_pos_player, radius_control_player_position, glm::vec4(0.3f, 0.3f, 1.f, 0.8f), TranslateGlobalToScreen);

		quads.AddLine({ global_left_border_player, -1.f }, { global_left_border_player, 1.f }, 10.f, glm::vec4(1.f, 0.f, 0.f, 0.5f), TranslateGlobalToScreen);
		quads.AddLine({ global_right_border_player,-1.f }, { global_right_border_player,1.f }, 10.f, glm::vec4(1.f, 0.f, 0.f, 0.5f), TranslateGlobalToScreen);

		quads.AddLine({ -10.f, global_pos_player.y }, { 10.f, global_pos_player.y }, 3.f, glm::vec4(1.f, 0.4f, 0.4f, 0.8f), TranslateGlobalToScreen);

		player.DrawDebug(quads);
	}

	if (flag_MODE_CreatorDestroyableObject) {
		if (index_destroyable_choosed != -1) {
			glm::vec4 color(1.f, 0.3f, 0.3f, 1.f);
			for (size_t i = 0; i < mesh_destroyables[index_destroyable_choosed].size(); i++)
			{
				circles.Add(mesh_destroyables[index_destroyable_choosed][i], radius_control_points, color, TranslateGlobalToScreen);
			}

			destroyable[index_destroyable_choosed].DrawDebug(quads);

		}
	}

	if (flag_MODE_EditBallPosition) {

		glm::vec4 color_pos(1.f, 0.3f, 0.3f, 1.f);
		if (ball_pos_hovered) color_pos.r = 0.1f;

		glm::vec4 color_tangent(0.f, 0.2f, 1.0f, 1.f);
		if (ball_tangent_hovered) color_tangent.b = 0.1f;

		circles.Add(ballSpawnPosition.globalPos, radius_control_points, color_pos, TranslateGlobalToScreen);


		circles.Add(ballSpawnPosition.globalPos, TranslateScalar_GlobalToScreen(ballSpawnPosition.global_radius), glm::vec4(0.1f, 0.1f, 1.f, 0.2f), TranslateGlobalToScreen);

		circles.Add(ballSpawnPosition.globalPos + ballSpawnPosition.tangent, radius_control_points, color_tangent, TranslateGlobalToScreen);

		quads.AddLine(ballSpawnPosition.globalPos, ballSpawnPosition.globalPos + ballSpawnPosition.tangent, glm::vec4(0.f, 1.f, 0.f, 1.f), TranslateGlobalToScreen);
	}


	levelBorder.Draw(quads, triangles);
	player.Draw(quads);
	for (size_t i = 0; i < destroyable.size(); i++)
		destroyable[i].Draw(quads, triangles);

	

	 




	if (ImGui::Begin("Level Creator",nullptr)) {

		if (ImGui::Button("Save")) {
			Save();
		}
		ImGui::SameLine();
		if (ImGui::Button("Load")) {
			Load();
		}
		ImGui::SameLine();
		if (ImGui::Button("Play")) {

		}


		if (ImGui::RadioButton("BorderMode", flag_MODE_CreatorBorder)) {
			flag_MODE_CreatorBorder = !flag_MODE_CreatorBorder;
			flag_MODE_EditPlayer = false;
			flag_MODE_CreatorDestroyableObject = false;
			flag_MODE_EditBallPosition = false;
		}
		if (ImGui::RadioButton("BallEditorMode", flag_MODE_EditBallPosition)) {
			flag_MODE_EditBallPosition = !flag_MODE_EditBallPosition;
			flag_MODE_EditPlayer = false;
			flag_MODE_CreatorBorder = false;
			flag_MODE_CreatorDestroyableObject = false;
		}
		if (flag_MODE_EditBallPosition)
		{
			if (ImGui::SliderFloat("Radius", &ballSpawnPosition.global_radius, 0.001f, 0.2f)) {
				player.UpdateRadius(ballSpawnPosition.global_radius);
				levelBorder.UpdateRadius(ballSpawnPosition.global_radius);

				for (size_t i = 0; i < destroyable.size(); i++)
					destroyable[i].UpdateRadius(ballSpawnPosition.global_radius);

			}

		}


		if (ImGui::RadioButton("PlayerEditorMode", flag_MODE_EditPlayer)) {
			flag_MODE_EditPlayer = !flag_MODE_EditPlayer;
			flag_MODE_EditBallPosition = false;
			flag_MODE_CreatorBorder = false;
			flag_MODE_CreatorDestroyableObject = false;
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
		if (ImGui::RadioButton("DestroyableCreatorMode", flag_MODE_CreatorDestroyableObject)) {
			flag_MODE_CreatorDestroyableObject = !flag_MODE_CreatorDestroyableObject;
			flag_MODE_EditPlayer = false;
			flag_MODE_CreatorBorder = false;
			flag_MODE_EditBallPosition = false;
		}

		if (flag_MODE_CreatorDestroyableObject) {

			for (size_t i = 0; i < choosedForEdit.size(); i++) {
				if (ImGui::RadioButton(("Edit ##" + std::to_string(i)).c_str(), choosedForEdit[i])) {

					for (size_t k = 0; k < choosedForEdit.size(); k++) {
						if (i != k) {
							choosedForEdit[k] = false;
						}
						index_destroyable_choosed = -1;

					}
					if (choosedForEdit[i] == 0) {
						choosedForEdit[i] = 1;
						index_destroyable_choosed = i;
					}
					else {
						choosedForEdit[i] = 0;
						index_destroyable_choosed = -1;
					}
				}
			}

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 10.f);
			if (ImGui::Button("ADD destroyable")) {

				DestroyableObject temp;

				destroyable.push_back(temp);
				choosedForEdit.push_back(0);
				mesh_destroyables.push_back(std::vector<glm::vec2>{});
			}

		}


		ImGui::End();
	}
}


void LevelCreator::BorderEditor_AddNewVertex(const glm::vec2& vertex) {


	if (vertices_border.size() < 3){
		vertices_border.push_back(vertex);
		levelBorder.SetVertices(vertices_border, ballSpawnPosition.global_radius);

		return;
	}

	glm::vec2 closestPoint = vertex;
	float minDistance = FLT_MAX;
	int index_cadidate = -1;

	for (size_t i = 0; i < vertices_border.size(); ++i) {
		size_t j = (i + 1) % vertices_border.size();
		glm::vec2 candidate = closestPointOnSegment(vertices_border[i], vertices_border[j], vertex);
		float distance = glm::length(candidate - vertex);

		if (distance < minDistance) {
			minDistance = distance;
			closestPoint = candidate;
			index_cadidate = i;
		}

	}

	int i = index_cadidate, 
		j = (index_cadidate + 1) % vertices_border.size();


	if ((std::min)(i, j) == 0 && (std::max)(i, j) == vertices_border.size() - 1) {
		vertices_border.push_back(vertex);
	}
	else {
		int index = (std::min)(i, j);
		vertices_border.insert(vertices_border.begin() + index + 1, vertex);
	}



	levelBorder.SetVertices(vertices_border, ballSpawnPosition.global_radius);

}

void LevelCreator::BorderEditor_Hover_and_DragVertex(const glm::vec2& newPos) {

	index_vertex_cover = -1;
	for (size_t i = 0; i < vertices_border.size(); i++) {

		if (Collision_Point_and_Circle(newPos, vertices_border[i], TranslateScalar_ScreenToGlobal(radius_control_points))) {

			index_vertex_cover = i;

			if (engine::input::IsMousePressed(MOUSE_LEFT)) {
				index_vertex_grab = i;
				break;
			}

			if (engine::input::IsKeyPressed(KEY_X)) {
				vertices_border.erase(vertices_border.begin() + i);
				levelBorder.SetVertices(vertices_border, ballSpawnPosition.global_radius);
				break;
			}
		}

	}

	if (engine::input::IsMouseDown(MOUSE_LEFT) && index_vertex_grab != -1) {
		vertices_border[index_vertex_grab] = newPos;

		levelBorder.SetVertices(vertices_border, ballSpawnPosition.global_radius);
	}
	else {
		index_vertex_grab = -1;
	}
}


void LevelCreator::Update() {
	
	player.Update();

	glm::vec2 mouse_global = TranslateScreenToGlobal(engine::input::GetMouseWindow());

	if (flag_MODE_CreatorBorder) {

		if (engine::input::IsMousePressed(MOUSE_RIGHT))
			BorderEditor_AddNewVertex(mouse_global);

		BorderEditor_Hover_and_DragVertex(mouse_global);

	}
	else if (flag_MODE_EditBallPosition) {
		ball_pos_hovered = false;
		ball_tangent_hovered = false;

		if (Collision_Point_and_Circle(mouse_global, ballSpawnPosition.globalPos, TranslateScalar_ScreenToGlobal(radius_control_points))) {
			ball_pos_hovered = true;

			if (engine::input::IsMousePressed(MOUSE_LEFT)) {
				ball_pos_draging = true;
			}
		}
		else if (Collision_Point_and_Circle(mouse_global, ballSpawnPosition.globalPos + ballSpawnPosition.tangent, TranslateScalar_ScreenToGlobal(radius_control_points))) {
			ball_tangent_hovered = true;

			if (engine::input::IsMousePressed(MOUSE_LEFT)) {
				ball_tangent_draging = true;
			}
		}


		if (engine::input::IsMouseDown(MOUSE_LEFT) && ball_pos_draging) {
			ballSpawnPosition.globalPos = mouse_global;
		}
		else
			ball_pos_draging = false;


		if (engine::input::IsMouseDown(MOUSE_LEFT) && ball_tangent_draging) {
			ballSpawnPosition.tangent = mouse_global- ballSpawnPosition.globalPos;
		}
		else
			ball_tangent_draging = false;


	}
	else if (flag_MODE_EditPlayer) {


		player_hovered = false;
		
		if (Collision_Point_and_Circle(mouse_global, global_pos_player, TranslateScalar_ScreenToGlobal(radius_control_player_position))) {
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
	else if (flag_MODE_CreatorDestroyableObject) {

		if (index_destroyable_choosed != -1) {


			if (engine::input::IsMousePressed(MOUSE_RIGHT)) {
				mesh_destroyables[index_destroyable_choosed].push_back(mouse_global);

				destroyable[index_destroyable_choosed].SetMesh(mesh_destroyables[index_destroyable_choosed], ballSpawnPosition.global_radius);
			}

			index_destroyable_cover_vertex = -1;
			for (size_t i = 0; i < mesh_destroyables[index_destroyable_choosed].size(); i++) {

				if (Collision_Point_and_Circle(mouse_global, mesh_destroyables[index_destroyable_choosed][i], TranslateScalar_ScreenToGlobal(radius_control_points))) {

					index_destroyable_cover_vertex = i;

					if (engine::input::IsMousePressed(MOUSE_LEFT)) {
						index_destroyable_grab_vertex = i;
						break;
					}

					if (engine::input::IsKeyPressed(KEY_X)) {
						mesh_destroyables[index_destroyable_choosed].erase(mesh_destroyables[index_destroyable_choosed].begin() + i);
						break;
					}
				}

			}

			if (index_destroyable_grab_vertex != -1){
				if (engine::input::IsMouseDown(MOUSE_LEFT) && index_destroyable_grab_vertex != -1) {
					mesh_destroyables[index_destroyable_choosed][index_destroyable_grab_vertex] = mouse_global;

					destroyable[index_destroyable_choosed].SetMesh(mesh_destroyables[index_destroyable_choosed], ballSpawnPosition.global_radius);
				}
				else {
					index_destroyable_grab_vertex = -1;
				}
			}


			if (index_destroyable_grab_vertex == -1)
			{

				static glm::vec2 global_pos_grab = { 0.f,0.f };
				if (Collision_Point_and_AABB(mouse_global, destroyable[index_destroyable_choosed].aabb)) {
					index_destroyable_cover = index_destroyable_choosed;

					if (engine::input::IsMousePressed(MOUSE_LEFT)) {
						index_destroyable_grab = index_destroyable_choosed;
						global_pos_grab = mouse_global;
						mesh_movable = mesh_destroyables[index_destroyable_choosed];
					}

				}
				else {
					index_destroyable_cover = -1;
				}




				if (engine::input::IsMouseDown(MOUSE_LEFT) && index_destroyable_grab != -1) {

					glm::vec2 delta_mouse = mouse_global - global_pos_grab;

					for (size_t i = 0; i < mesh_movable.size(); i++)
					{
						mesh_destroyables[index_destroyable_choosed][i] = mesh_movable[i] + delta_mouse;
					}

					destroyable[index_destroyable_choosed].SetMesh(mesh_destroyables[index_destroyable_choosed], ballSpawnPosition.global_radius);
				}
				else {
					index_destroyable_grab = -1;
				}

			}


		}





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

		for (size_t i = 0; i < destroyable.size(); i++)
			data["destroyable"][i] = destroyable[i].Save();

		data["ballSpawn"] = ballSpawnPosition.Save();



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

		if (data.contains("ballSpawn")) {
			ballSpawnPosition.Load(data["ballSpawn"]);
		}

		if (data.contains("border")) {
			levelBorder.Load(data["border"]);

			vertices_border.clear();

			const auto& vertices_level = levelBorder.GetVertices_OriginalBorder();
			vertices_border.resize(vertices_level.size() - 1);

			for (size_t i = 0; i < vertices_level.size() - 1; i++)
				vertices_border[i] = vertices_level[i];

			levelBorder.UpdateRadius(ballSpawnPosition.global_radius);
		}

		if (data.contains("player")) {
			player.Load(data["player"]);

			global_left_border_player = player.GetLeftBound();
			global_right_border_player = player.GetRightBound();

			global_pos_player = player.GetBeginPosition();

			player.UpdateRadius(ballSpawnPosition.global_radius);

		}

		if (data.contains("destroyable")) {

			destroyable.clear();
			mesh_destroyables.clear();

			for (const auto& [key, destroyableData] : data["destroyable"].items()) {
				DestroyableObject temp;
				temp.Load(destroyableData);

				temp.UpdateRadius(ballSpawnPosition.global_radius);

				destroyable.emplace_back(temp);

				mesh_destroyables.emplace_back(temp.mesh);
				if (!mesh_destroyables.back().empty())
					mesh_destroyables.back().pop_back();
				



			}

			choosedForEdit.clear();
			choosedForEdit.resize(mesh_destroyables.size(),false);


		}



	}
}