#ifndef GAME_COMBO_H
#define GAME_COMBO_H

#include "Core/Font/Font.Atlas.h"
#include "Core/Text/Text.Atlas.h"
#include "Rendering/RenderObjects/TextInstanced/Render.TextInstanced.h"


class GameCombo
{
public:
	GameCombo();
	~GameCombo();

	void Draw(TextInstanced& renderer, FontAtlas& font,const glm::vec2& pos,const float& size);
	void Update();

	void AddCombo();

	float GetValue();

private:
	uint64_t value = 0;

	float timeLife = 1.2f;
	float timeLastCombo = -timeLife;

	float getTime();
	float getNormalizedTime();

	bool isHide();
};



#endif