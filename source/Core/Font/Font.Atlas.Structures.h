#ifndef FONT_ATLAS_STRUCTURES_H
#define FONT_ATLAS_STRUCTURES_H


#include <cstdint>
#include <string>
#include <nlohmann/json.hpp>

struct FontAtlas_Info {
	enum TypeAtlas {
		HARD_MASK,
		SOFT_MASK,
		SDF,
		PSDF,
		MSDF,
		MTSDF
	};

	enum Type_Y_Origin {
		BOTTOM,
		TOP
	};

	static TypeAtlas     str_to_typeAtlas(const std::string&);
	static Type_Y_Origin str_to_typeYorigin(const std::string&);


	TypeAtlas type;
	float distanceRange;
	float distanceRangeMiddle;
	float size;
	float width;
	float height;
	Type_Y_Origin yOrigin;
};

struct FontAtlas_Metrics {
	float emSize;
	float lineHeight;
	float ascender;
	float descender;
	float underlineY;
	float underlineThickness;
};

struct FontAtlas_Bounds {
	float left;
	float bottom;
	float right;
	float top;

	static FontAtlas_Bounds json_to_bounds(const nlohmann::json&);

	void normalize(const float& maxValue);

};

struct FontAtlas_Glyph {
	float advance;
	FontAtlas_Bounds planeBounds;
	FontAtlas_Bounds atlasBounds;
};


#endif