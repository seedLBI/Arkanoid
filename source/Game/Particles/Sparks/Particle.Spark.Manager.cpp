#include "Particle.Spark.Manager.h"

#include "Core/CoordinateSystem/CoordinateSystem.h"

#include "Game/Math/Arkanoid_Math.h"
#include <random>

ParticleSparkManager::ParticleSparkManager() {

}

ParticleSparkManager::~ParticleSparkManager() {

}

void ParticleSparkManager::Draw(QuadInstanced& quad_renderer) {
	for (size_t i = 0; i < particles.size(); i++) {
		ParticleSpark& p = particles[i];

		glm::vec2& begin = p.pos;
		glm::vec2& end   = p.lastPos;
		glm::vec4& color = p.color;
		float& thickness = p.size;

		quad_renderer.AddLine(begin, end, thickness, color, TranslateGlobalToScreen);
	}

}

void ParticleSparkManager::Update() {
	for (size_t i = 0; i < particles.size(); i++) {

		ParticleSpark& p = particles[i];


		if (p.lastPos != p.pos) {
			float length_with_last = glm::length(p.lastPos - p.pos);
			if (length_with_last > p.length) {
				p.lastPos = p.pos - p.dir * p.length;
			}
		}

		p.pos += p.dir * engine::time::GetDeltaTime() * p.speed;


		float time_alive = (engine::time::GetProgrammTime() - p.timeCrated);

		float time_fade = 1.f - time_alive / p.timeLife;

		p.color.a = time_fade;

		if (time_alive > p.timeLife) {
			particles.erase(particles.begin() + i);
			i--;
		}
	}
}

void ParticleSparkManager::Add(const glm::vec2& pos, const glm::vec2& normal, const int& count, const glm::vec4& color) {

	constexpr float min_angle = -glm::pi<float>()/2.3f;
	constexpr float max_angle =  glm::pi<float>()/2.3f;
	constexpr float range_angle = max_angle - min_angle;

	const float min_speed = 3.2f;
	const float max_speed = 7.7f;
	const float range_speed = max_speed - min_speed;


	for (size_t i = 0; i < count; i++) {
		float rand_value = (float)rand() / (float)RAND_MAX;

		float rand_maped_angle = (rand_value * range_angle) + min_angle;
		float rand_maped_speed = (rand_value * range_speed) + min_speed;

		ParticleSpark new_particle;
		
		new_particle.dir		= glm::normalize(rotate(normal, rand_maped_angle));
		new_particle.speed		= rand_maped_speed;
		new_particle.pos		= pos;
		new_particle.lastPos	= pos;
		new_particle.color		= color;
		new_particle.timeCrated = engine::time::GetProgrammTime();

		new_particle.length		= 0.2f;
		new_particle.size		= 2.f;
		new_particle.timeLife	= 0.2f;


		particles.push_back(new_particle);
	}

}