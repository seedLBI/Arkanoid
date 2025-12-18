#ifndef FONT_ATLAS_H
#define FONT_ATLAS_H

#include "Font.Atlas.Structures.h"
#include <nlohmann/json.hpp>
#include <robin_hood.h>
#include <cstdint>
#include <vector>
#include <string>

#include "Utils/OpenGL/Texture/Texture.h"



class FontAtlas {
public:
	FontAtlas();
	~FontAtlas();

	void Load(const std::string& path2file_without_extension);

	const FontAtlas_Glyph&   getGlyph(const uint64_t& unicode);
		  Texture&		     getTexture();
	const FontAtlas_Info&    getAtlasInfo();
	const FontAtlas_Metrics& getMetrics();

private:
	FontAtlas_Info atlas;
	FontAtlas_Metrics metrics;
	Texture texture;
	robin_hood::unordered_flat_map<uint64_t, FontAtlas_Glyph> glyphs;


	void parseJson(const nlohmann::json& json);
};




#endif