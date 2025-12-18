#ifndef TEXT_ATLAS_OBJECT_H
#define TEXT_ATLAS_OBJECT_H


#include "Rendering/RenderObjects/TextInstanced/Render.TextInstanced.h"
#include "Core/Font/Font.Atlas.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

struct TextAtlas {
	std::vector<uint64_t> text;

	glm::vec4 color_base;
	glm::vec4 color_outline;

	float thickness_base;
	float thickness_outline;

	glm::vec2 getSize(FontAtlas& atlas, const float& size, const float& spacing);

	void addToRender(
		TextInstanced& renderer, 
		FontAtlas& atlas, 
		const glm::vec2& pos, 
		const float& size, 
		const float& spacing);
};

TextAtlas makeText(const std::string& text, glm::vec4 color_base, glm::vec4 color_outline);

#endif