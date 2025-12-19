#include "Particle.Text.Manager.h"


#include "Core/CoordinateSystem/CoordinateSystem.h"

void ParticleTextManager::Draw(TextInstanced& text_renderer, FontAtlas& font) {

	for (size_t i = 0; i < particles.size(); i++) {
		
		float size_screen = TranslateScalar_GlobalToScreen(particles[i].size_global);



		glm::vec2 pos_screen = TranslateGlobalToScreen(particles[i].pos);

		glm::vec2 size_text = particles[i].text.getSize(font, size_screen, 0.f);
		size_text.y *= -1.f;


		particles[i].text.addToRender(
			text_renderer,
			font,
			pos_screen - size_text/2.f,
			size_screen,
			0.f);

	}

}
void ParticleTextManager::Update() {
	for (size_t i = 0; i < particles.size(); i++) {

		ParticleText& p = particles[i];

		p.pos += p.direction * engine::time::GetDeltaTime() * p.speed;

		float time_alive = (engine::time::GetProgrammTime() - p.timeCreated);

		float time_fade = 1.f - time_alive / p.timeLife;

		p.text.color_base.a = time_fade;

		if (time_alive > p.timeLife) {
			particles.erase(particles.begin() + i);
			i--;
		}
	}
}


void ParticleTextManager::Add(
	const std::string& text,
	const float& size_global,
	const glm::vec2& pos_global, 
	const glm::vec2& dir, 
	const glm::vec4& color_base){

	ParticleText new_particle;

	new_particle.text = makeText(text, color_base, glm::vec4(0.f));
	new_particle.pos = pos_global;
	new_particle.direction = glm::normalize(dir);
	new_particle.size_global = size_global;
	new_particle.speed = 0.3f;
	new_particle.timeCreated = glfwGetTime();
	new_particle.timeLife = 1.f;
	
	particles.push_back(new_particle);
}

ParticleTextManager::ParticleTextManager() {

}

ParticleTextManager::~ParticleTextManager() {

}
