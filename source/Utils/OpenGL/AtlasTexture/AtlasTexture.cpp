#include "AtlasTexture.h"


AtlasTexture::AtlasTexture(const std::string& pathData) {

	//


	std::vector<std::string> paths2images;
	std::vector<nlohmann::json> jsons_data;

	const std::string& path2json = pathData;

	nlohmann::json data = nlohmann::json::parse(std::ifstream(path2json));


	if (data.contains("atlases")) {
		for (const auto& [key, value]: data["atlases"].items()) {
			paths2images.push_back("resources/img/atlases/" + value.get<std::string>());
		}
	}
	else
		assert("WHERE PATH TO IMAGE IN JSON ATLAS????\n");

	data.erase("atlases");

	jsons_data.push_back(data);
	

	InitTexture(paths2images);

	for (size_t i = 0; i < jsons_data.size(); i++)
	{
		std::string name;
		InitData(jsons_data[i], name, i);
	}



	names.reserve(objects.size());
	for (auto& [name, object] : objects) {
		names.emplace_back(name);
		printf("[%i][%s]\n", object.index_layer, name.c_str());
	}
	std::sort(names.begin(), names.end());
	
	printf("Readed atlas data objects:\n");
	for (const auto& name: names) {
		printf("[%s]\n", name.c_str());
	}

	
}

const std::vector<std::string>& AtlasTexture::GetNames() {
	return names;
}
robin_hood::unordered_flat_map<std::string, AtlasObject>& AtlasTexture::GetObjects() {
	return objects;
}


AtlasTexture::~AtlasTexture() {

}

Texture2DArrays& AtlasTexture::GetTexture() {
	return texture;
}

void AtlasTexture::InitTexture(const std::vector<std::string>& pathTexture) {
	texture.LoadImages(pathTexture);
	texture.Init();
}

void AtlasTexture::InitData(nlohmann::json& data_json, std::string& name, const int& index_atlas) {
	for (auto& [key, data_] : data_json.items()) {
		std::string current_name = name.empty() ? key : name + "::" + key;


		if (data_.contains("x")) {
			AtlasObject temp;
			temp.width = data_["width"].get<int>();
			temp.height = data_["height"].get<int>();

			temp.x = data_["x"].get<int>();
			temp.y = texture.getHeight() - data_["y"].get<int>() - temp.height;


			if(data_.contains("columns"))
				temp.cols = data_["columns"].get<int>();
			else
				temp.cols = 1;

			if(data_.contains("rows"))
				temp.rows = data_["rows"].get<int>();
			else
				temp.rows = 1;

			temp.sprite_width = temp.width / temp.cols;
			temp.sprite_height = temp.height / temp.rows;

			temp.uv_sprite_size = {
				(float)temp.sprite_width / (float)texture.getWidth(),
				(float)temp.sprite_height / (float)texture.getHeight()
			};

			temp.uv_min = {
				(float)temp.x / (float)texture.getWidth(),
				(float)temp.y / (float)texture.getHeight(),
			};

			temp.uv_max = {
				float(temp.x + temp.width) / (float)texture.getWidth(),
				float(temp.y + temp.height) / (float)texture.getHeight()
			};

			temp.uv_size = {
				float(temp.width) / (float)texture.getWidth(),
				float(temp.height) / (float)texture.getHeight()
			};

			temp.index_layer = data_["atlas"].get<int>();


			objects[current_name] = temp;
		}
		else {
			InitData(data_, current_name,index_atlas);
		}
	}
}
