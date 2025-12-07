#ifndef KEYFRAMES_H
#define KEYFRAMES_H

#include "Core/Animation/Keyframe/Keyframe.h"
#include <vector>
#include <nlohmann/json.hpp>


struct Keyframes {

	Keyframes();
	Keyframes(const float& default_value);

	std::vector<Keyframe> keys;
	std::vector<glm::vec2> samples;

	int min_sampleRate = 2;
	int max_sampleRate = 30;

	void setDefaultValue(const float& value);

	void correctAllKeys();
	void discretize();
	float evaluate(const float& time) const;

	void addPoint(const glm::vec2& p);

	float getBeginTime();
	float getEndTime();
	void  addDeltaTime(const float& time);

	nlohmann::json Save();
	void Load(const nlohmann::json&);


private:
	glm::vec2 lerp(const Keyframe& k1, const Keyframe& k2, const float& t);

	float default_value = 0.f;

};


#endif