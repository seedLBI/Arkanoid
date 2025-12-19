#ifndef PARTICLE_SPARK_MANAGER_H
#define PARTICLE_SPARK_MANAGER_H

#include "Core/Engine/Engine.h"
#include "Particle.Spark.h"

#include "Rendering/RenderObjects/QuadInstanced/Render.Quad.Instanced.h"

#include <vector>


class ParticleSparkManager {
public:
	ParticleSparkManager();
	~ParticleSparkManager();

	void Draw(QuadInstanced& quad_renderer);
	void Update();

	void Add(
		const glm::vec2& pos, 
		const glm::vec2& normal, 
		const int& count, 
		const glm::vec4& color);

private:
	std::vector<ParticleSpark> particles;
};



#endif