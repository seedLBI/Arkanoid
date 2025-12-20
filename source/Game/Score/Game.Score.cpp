#include "Game/Score/Game.Score.h"

#include "Core/CoordinateSystem/CoordinateSystem.h"
#include "Utils/Text/Text.h"

#include <string>


void GameScore::Update() {

	float time_elapsed_after_addScore = engine::time::GetProgrammTime() - timeLastAdd;
	if (time_elapsed_after_addScore > timeFor_addScore_shows) {
		//animatedScore = score;
		score += scoreAdd;
		scoreAdd = 0.0;
		flag_draw_addScore = false;


	}
	else {
		flag_draw_addScore = true;
	}


	animatedScore = animatedScore + engine::time::GetDeltaTime() * 10.f * ((score + 0.5) - animatedScore);


}

void GameScore::Draw(QuadInstanced& quads_renderer, TextInstanced& text_renderer, FontAtlas& font, const glm::vec2& pos, const float& size) {

	std::string value = std::to_string(animatedScore);
	DeleteAllFromFirstFindSymbol(value, '.');
	PadLeft(value, '0', 6);

	TextAtlas score = makeText(value,glm::vec4(1.f), glm::vec4(0.f));
	glm::vec2 size_score = score.getSize(font, size, 0.f);

	float y_pos_adjust = -font.getMetrics().ascender;

	glm::vec2 add{ size * 0.25,size };

	score.addToRender(text_renderer, font, pos + add, size, 0.f);

	if (flag_draw_addScore) {

		float time_anim_addScore = (engine::time::GetProgrammTime() - timeLastAdd) / timeFor_addScore_shows;

		

		float alpha_anim = glm::smoothstep(1.f, 0.8f, time_anim_addScore);
		float pos_anim = glm::smoothstep(0.f, 0.2f, time_anim_addScore)*0.5 + glm::smoothstep(0.9f, 1.f, time_anim_addScore) * 0.5f;


		glm::vec2 begin = TranslateScreenToGlobal(pos + glm::vec2{ 0.f, size * 1.05f });
		glm::vec2 end   = TranslateScreenToGlobal(pos + glm::vec2{size_score.x, size * 1.05f });

		float thickness = size * 0.1f;

		glm::vec4 color_text = glm::vec4(1.f, 0.4f, 0.f, 1.f);
		glm::vec4 color_line = glm::vec4(1.f);
		color_line.a = alpha_anim;
		color_text.a = alpha_anim;


		quads_renderer.AddLine(begin, end, thickness, color_line, TranslateGlobalToScreen);

		std::string add_value = std::to_string(scoreAdd);
		DeleteAllFromFirstFindSymbol(add_value, '.');
		add_value = "+" + add_value;


		TextAtlas addScore = makeText(add_value, color_text, glm::vec4(0.f));
		glm::vec2 size_addScore = addScore.getSize(font, size, 0.f);


		add = glm::vec2{ size * 0.25 + size_score.x - size_addScore.x,size* (2.4f - pos_anim) };

		addScore.addToRender(text_renderer, font, pos + add, size, 0.f);


	}


}

void GameScore::AddScore(const long double& value) {
	timeLastAdd = engine::time::GetProgrammTime();

	flag_draw_addScore = true;

	scoreAdd += value;

	if (scoreAdd < 0.0)
		scoreAdd = 0.0;
	if (scoreAdd > maxScore)
		scoreAdd = maxScore;
}

GameScore::GameScore() {

}

GameScore::~GameScore() {

}

