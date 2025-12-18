#include "Font.Atlas.Structures.h"

#include <robin_hood.h>

FontAtlas_Info::TypeAtlas FontAtlas_Info::str_to_typeAtlas(const std::string& str_type) {

	robin_hood::unordered_flat_map<std::string, TypeAtlas> translator = {
		{"hardmask", TypeAtlas::HARD_MASK},
		{"softmask", TypeAtlas::SOFT_MASK},
		{"sdf",		 TypeAtlas::SDF},
		{"psdf",	 TypeAtlas::PSDF},
		{"msdf",	 TypeAtlas::MSDF},
		{"mtsdf",	 TypeAtlas::MTSDF}
	};

	if (translator.contains(str_type))
		return translator[str_type];

	throw std::runtime_error("FontAtlas_Info::str_to_typeAtlas(const std::string& str_type) - UNKNOWN STR");
	return TypeAtlas();
}


FontAtlas_Info::Type_Y_Origin FontAtlas_Info::str_to_typeYorigin(const std::string& str_type) {

	robin_hood::unordered_flat_map<std::string, Type_Y_Origin> translator = {
		{"bottom",  Type_Y_Origin::BOTTOM},
		{"top",		Type_Y_Origin::TOP},
	};

	if (translator.contains(str_type))
		return translator[str_type];

	throw std::runtime_error("FontAtlas_Info::str_to_typeYorigin(const std::string&) - UNKNOWN STR");
	return Type_Y_Origin();
}


FontAtlas_Bounds FontAtlas_Bounds::json_to_bounds(const nlohmann::json& jsonData) {
	if (!jsonData.contains("left"))
		throw std::runtime_error("json don't contain left");
	if (!jsonData.contains("bottom"))
		throw std::runtime_error("json don't contain bottom");
	if (!jsonData.contains("right"))
		throw std::runtime_error("json don't contain right");
	if (!jsonData.contains("top"))
		throw std::runtime_error("json don't contain top");

	FontAtlas_Bounds output;
	output.left   = jsonData["left"].get<float>();
	output.bottom = jsonData["bottom"].get<float>();
	output.top    = jsonData["top"].get<float>();
	output.right  = jsonData["right"].get<float>();

	return output;
}
