#include "Particle.Spark.Manager.h"

#include <random>

ParticleSparkManager::ParticleSparkManager() {

}

ParticleSparkManager::~ParticleSparkManager() {

}

void ParticleSparkManager::Draw() {

}

void ParticleSparkManager::Update() {
	for (size_t i = 0; i < particles.size(); i++) {

		ParticleSpark& p = particles[i];

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

	const float min_angle = -glm::pi<float>()/2.3f;
	const float max_angle =  glm::pi<float>()/2.3f;

	const float range_angle = max_angle - min_angle;





}