#include "Game.Combo.h"

#include "Game/Math/Arkanoid_Math.h"
void GameCombo::Draw(TextInstanced& renderer, FontAtlas& font, const glm::vec2& pos, const float& size) {
	if (isHide())
		return;

	TextAtlas text_combo = makeText(u8"Комбо");
	TextAtlas text_value = makeText(std::to_string(value));

	float time = getNormalizedTime();


	float color_mix_time = glm::smoothstep(1.f, 10.f, GetValue());
	text_value.color_base = glm::mix(glm::vec4(1.f), glm::vec4(1.f, 0.2f, 0.2f, 1.f), color_mix_time);

	float anim_alpha = glm::smoothstep(1.f, 0.8f, time);
	text_combo.color_base.a = anim_alpha;
	text_value.color_base.a = anim_alpha;





	float size_value = size * (1.f - time) * (1.5f + GetValue() * 0.2f);
	size_value = glm::clamp(size_value, size, size * 2.f);
	
	glm::vec2 size_text_combo = text_combo.getSize(font, size,       0.f);


	glm::vec2 circle = size * 0.15f *  getPointOnCircle(engine::time::GetProgrammTime() * 20.f) * (1.f-time*time*time);
	circle.x *= circle.x*0.125f;

	text_combo.addToRender(renderer, font, circle + pos - glm::vec2{ size_text_combo.x * 0.9f, -size_text_combo.y / 2.f }, size, 0.f);


	float angle = 0.2f*(-sqrtf(time) + 1.f) * (sinf((26.2f + GetValue()) * time + 1.47f)) * glm::smoothstep(0.6f, 0.f, time);


	auto data = text_value.getSizeAndCenter(font, size_value, 0.f);
	glm::vec2& size_text_value   = data.first;
	glm::vec2& center_text_value = data.second;

	text_value.addToRender(renderer, font, circle + pos - glm::vec2{0.f, -size_text_value.y/2.f}, center_text_value, angle, size_value, 0.f);
}


float GameCombo::getTime() {
	return engine::time::GetProgrammTime() - timeLastCombo;
}
float GameCombo::getNormalizedTime() {
	return getTime() / timeLife;
}


void GameCombo::Update() {
	if (isHide())
		value = 0.f;



}

float GameCombo::GetValue() {
	return (float)value;
}

void GameCombo::AddCombo() {
	value++;
	timeLastCombo = engine::time::GetProgrammTime();
}

bool GameCombo::isHide() {
	return getTime() > timeLife;
}


GameCombo::GameCombo() {

}

GameCombo::~GameCombo() {

}