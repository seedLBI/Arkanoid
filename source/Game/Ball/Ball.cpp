#include "Ball.h"

#include "Game/Math/Arkanoid_Math.h"
#include "Core/CoordinateSystem/CoordinateSystem.h"

void Ball::Draw(DebugCircle& renderer) {

	for (size_t i = 0; i < trail.size(); i++) {
		float time = (float)i / (float)trail.size();

		const glm::vec2& pos = trail[i].pos;

		float radiuss = TranslateScalar_GlobalToScreen(radius) * time;

		glm::vec4 ccolor = glm::vec4(color.r,color.g,color.b,0.5f) + (1.f - time * time) * (glm::vec4(0.4f, 0.2f, 1.f, 0.f) - glm::vec4(color.r, color.g, color.b, 0.5f));

		renderer.Add(pos, radiuss, ccolor, TranslateGlobalToScreen);
	}

	//renderer.Add(path.begin, TranslateScalar_GlobalToScreen(radius * 1.2f), glm::vec4(0.f,0.f,0.f,1.f), TranslateGlobalToScreen);
	renderer.Add(path.begin, TranslateScalar_GlobalToScreen(radius), color, TranslateGlobalToScreen);
}

void Ball::DrawDebug(DebugCircle& renderer, QuadInstanced& quads) {
	renderer.Add(path.end, TranslateScalar_GlobalToScreen(radius), glm::vec4(1.f,0.f,0.f,0.5f), TranslateGlobalToScreen);

	quads.AddLine(path.end, path.end + tangent, 2.f, glm::vec4(0.f, 1.f, 0.f, 1.f), TranslateGlobalToScreen);
}

void Ball::ResetAll() {
	trail.clear();
}

void Ball::Update() {
	TrailUpdate();
}

void Ball::TrailUpdate() {
	if (trail.empty() == false) {
		float len_last = glm::length(path.begin - trail.back().pos);

		if (!isnan(len_last)) {

			const float len_need = LengthTrail / (float)MAX_count_trail;

			int countAdd = len_last / len_need;
			const glm::vec2 begin = path.begin;
			const glm::vec2 end = trail.back().pos;
			for (int i = countAdd - 1; i >= 0; i--) {

				float time = (len_need * (float)i) / len_last;

				glm::vec2 pointAdd = lerp(begin, end, time);

				trail.push_back({ pointAdd, engine::time::GetProgrammTime() });
			}
		}
	}
	else {
		trail.push_back({ path.begin,engine::time::GetProgrammTime() });
	}


	for (size_t i = 0; i < trail.size(); i++) {
		auto& p = trail[i];
		const float currentTime = engine::time::GetProgrammTime();

		if (currentTime - p.timeCreation > LifeTimePoints) {
			trail.erase(trail.begin() + i);
			i--;
		}
	}
	if (trail.size() > MAX_count_trail) {
		trail.erase(trail.begin(), trail.end() - MAX_count_trail);
	}
}