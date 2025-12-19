#include "Text.Atlas.h"

glm::vec2 TextAtlas::getSize(FontAtlas& atlas, const float& size, const float& spacing) {

	if (text.empty())
		return glm::vec2{ 0.f,0.f };
	
	float y_min = std::numeric_limits<float>::max();
	float y_max = std::numeric_limits<float>::min();

	float x = 0.f;
	for (size_t i = 0; i < text.size(); i++) {
		const auto& glyph = atlas.getGlyph(text[i]);
		x += glyph.advance * size + spacing;

		y_max = (std::max)(y_max, glyph.planeBounds.top    * size);
		y_min = (std::min)(y_min, glyph.planeBounds.bottom * size);
	}
	x -= spacing;

	float x_min = atlas.getGlyph(text.front()).planeBounds.left;
	float x_max = x + atlas.getGlyph(text.back()).planeBounds.right;

	float width = x_max - x_min;
	float height = y_max - y_min;

	return glm::vec2{ width,height };

}

void TextAtlas::addToRender(
	TextInstanced& renderer,
	FontAtlas& atlas,
	const glm::vec2& pos,
	const float& size,
	const float& spacing) {

	glm::vec2 tangent = { 0.f,-1.f };
	glm::vec2 normal  = { 1.f,0.f };

	float x = 0.f;

	for (size_t i = 0; i < text.size(); i++) {
		const auto& glyph = atlas.getGlyph(text[i]);

		float pos_width_half = size * ((glyph.planeBounds.left + glyph.planeBounds.right) / 2.f);
		float pos_height_half = size * ((glyph.planeBounds.top + glyph.planeBounds.bottom) / 2.f);

		glm::vec2 glyph_pos = pos + glm::vec2{ x + pos_width_half, -pos_height_half };

		float width  = (glyph.planeBounds.right - glyph.planeBounds.left) * size;
		float height = (glyph.planeBounds.top - glyph.planeBounds.bottom) * size;

		glm::vec2 glyph_size = { width, height };


		glm::vec2 uv_min, uv_size;

		uv_min.x = glyph.atlasBounds.left;
		uv_size.x = glyph.atlasBounds.right - glyph.atlasBounds.left;

		uv_min.y  = glyph.atlasBounds.bottom;
		uv_size.y = (glyph.atlasBounds.top - glyph.atlasBounds.bottom);


		renderer.Add(glyph_pos, glyph_size, tangent, normal, uv_min, uv_size, this->color_base, this->color_outline);


		x += glyph.advance * size + spacing;
	}

}


#include "Utils/Text/Encoding/UTF8/UTF8.h"
#include "Utils/Text/Encoding/Unicode/Unicode.h"

TextAtlas makeText(const std::string& text, glm::vec4 color_base, glm::vec4 color_outline) {
	UTF8_SPLITER_ERROR error;
	auto splitted = utf8_splitter(text, error);

	std::vector<uint64_t> unicode_symbols(splitted.size());

	for (size_t i = 0; i < splitted.size(); i++) {
		unicode_symbols[i] = strUTF8Symbol_to_UnicodeValue(splitted[i]);
	}

	TextAtlas output;
	output.text = unicode_symbols;
	output.color_base = color_base;
	output.color_outline = color_outline;
	output.thickness_base = 0.f;
	output.thickness_outline = 0.f;

	return output;
}