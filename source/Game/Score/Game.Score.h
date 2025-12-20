#ifndef GAME_SCORE_H
#define GAME_SCORE_H

#include "Core/Engine/Engine.h"
#include "Core/Font/Font.Atlas.h"
#include "Core/Text/Text.Atlas.h"

#include "Rendering/RenderObjects/TextInstanced/Render.TextInstanced.h"
#include "Rendering/RenderObjects/QuadInstanced/Render.Quad.Instanced.h"


#include <cstdint>

class GameScore
{
public:
	GameScore();
	~GameScore();

	void AddScore(const long double& value);

	void Update();
	void Draw(QuadInstanced& quads_renderer, TextInstanced& text_renderer, FontAtlas& font, const glm::vec2& pos,const float& size);

private:

	float timeLastAdd = -2.f;
	
	float timeFor_addScore_shows = 1.f;



	bool flag_draw_addScore = false;



	long double maxScore = 999'999.0;


	long double scoreAdd = 0.0;

	long double score = 0.0;
	long double animatedScore = 0.0;
};




#endif