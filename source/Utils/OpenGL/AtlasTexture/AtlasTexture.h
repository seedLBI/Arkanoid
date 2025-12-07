#pragma once

#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "Utils/OpenGL/Texture2DArrays/Texture2DArrays.h"
#include <robin_hood.h>

struct AtlasObject {
	glm::vec2 uv_min;
	glm::vec2 uv_max;
	glm::vec2 uv_size;
	glm::vec2 uv_sprite_size;

	int x, y;
	int width, height;
	int cols, rows;
	int sprite_width, sprite_height;

	int index_layer;
};

class AtlasTexture {
public:
	AtlasTexture(const std::string& pathData);
	~AtlasTexture();

	Texture2DArrays& GetTexture();
	const std::vector<std::string>& GetNames();
	robin_hood::unordered_flat_map<std::string, AtlasObject>& GetObjects();

private:
	Texture2DArrays texture;
	
	robin_hood::unordered_flat_map<std::string, AtlasObject> objects;
	std::vector<std::string> names;

	void InitTexture(const std::vector<std::string>& pathTexture);
	void InitData(nlohmann::json& data_json, std::string& name, const int& index_atlas);
};

