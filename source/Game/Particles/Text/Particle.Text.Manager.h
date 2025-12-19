#ifndef PARTICLE_TEXT_MANAGER_H
#define PARTICLE_TEXT_MANAGER_H

#include <glm/glm.hpp>
#include "Core/Text/Text.Atlas.h"

#include "Rendering/RenderObjects/TextInstanced/Render.TextInstanced.h"
#include "Particle.Text.h"


#include <vector>
#include <string>


class ParticleTextManager {
public:
	ParticleTextManager();
	~ParticleTextManager();

	void Draw(TextInstanced& text_renderer, FontAtlas& font);
	void Update();

	void Add(
		const std::string& text, 
		const float& size_global,
		const glm::vec2& pos_global, 
		const glm::vec2& dir, 
		const glm::vec4& color_base);

private:
	std::vector<ParticleText> particles;
};


#endif // !PARTICLE_TEXT_MANAGER_H
