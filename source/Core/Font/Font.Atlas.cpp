#include "Font.Atlas.h"

#include <fstream>
	
void FontAtlas::Load(const std::string& path2file_without_extension) {
	std::ifstream file_json(path2file_without_extension + ".json");
	nlohmann::json json(nlohmann::json::parse(file_json));
	file_json.close();

	parseJson(json);

	texture.LoadTexture(path2file_without_extension + ".png", 4);

	Texture2DSetting tex_settings;
	tex_settings.Max = TextureFilter::LINEAR_MIPMAP_NEAREST;
	tex_settings.Min = TextureFilter::LINEAR_MIPMAP_NEAREST;
	tex_settings.WrapX = TextureWrap::CLAMP_TO_EDGE;
	tex_settings.WrapY = TextureWrap::CLAMP_TO_EDGE;

	texture.SetSetting(tex_settings);
	texture.Init();
}

void FontAtlas::parseJson(const nlohmann::json& json) {

	if (!json.contains("atlas"))
		throw std::runtime_error("Atlas json not have ATLAS");
	const auto& atlas_data = json["atlas"];
	this->atlas.type				= FontAtlas_Info::str_to_typeAtlas(atlas_data["type"].get<std::string>());
	this->atlas.yOrigin				= FontAtlas_Info::str_to_typeYorigin(atlas_data["yOrigin"].get<std::string>());
	this->atlas.distanceRange		= atlas_data["distanceRange"].get<float>();
	this->atlas.distanceRangeMiddle = atlas_data["distanceRangeMiddle"].get<float>();
	this->atlas.size				= atlas_data["size"].get<float>();
	this->atlas.width				= atlas_data["width"].get<float>();
	this->atlas.height				= atlas_data["height"].get<float>();



	if (!json.contains("metrics"))
		throw std::runtime_error("Atlas json not have METRICS");
	const auto& metrics_data = json["metrics"];
	this->metrics.emSize			 = metrics_data["emSize"].get<float>();
	this->metrics.lineHeight		 = metrics_data["lineHeight"].get<float>();
	this->metrics.ascender			 = metrics_data["ascender"].get<float>();
	this->metrics.descender			 = metrics_data["descender"].get<float>();
	this->metrics.underlineY		 = metrics_data["underlineY"].get<float>();
	this->metrics.underlineThickness = metrics_data["underlineThickness"].get<float>();



	if (!json.contains("glyphs"))
		throw std::runtime_error("Atlas json not have GLYPHS");
	const auto& glyphs_data = json["glyphs"];
	for (const auto& [i, glyphData] : glyphs_data.items()) {
		uint64_t unicode = glyphData["unicode"].get<uint64_t>();

		FontAtlas_Glyph glyph;
		glyph.advance = glyphData["advance"].get<float>();
		
		if (glyphData.contains("planeBounds"))
			glyph.planeBounds = FontAtlas_Bounds::json_to_bounds(glyphData["planeBounds"]);

		if (glyphData.contains("atlasBounds")) {
			glyph.atlasBounds = FontAtlas_Bounds::json_to_bounds(glyphData["atlasBounds"]);
			glyph.atlasBounds.normalize(this->atlas.height);
		}



		this->glyphs[unicode] = glyph;
	}

}

const FontAtlas_Glyph& FontAtlas::getGlyph(const uint64_t& unicode) {
	return this->glyphs[unicode];
}

Texture& FontAtlas::getTexture() {
	return texture;
}

const FontAtlas_Info& FontAtlas::getAtlasInfo() {
	return atlas;
}
const FontAtlas_Metrics& FontAtlas::getMetrics() {
	return metrics;
}


FontAtlas::FontAtlas() {

}

FontAtlas::~FontAtlas() {
}



