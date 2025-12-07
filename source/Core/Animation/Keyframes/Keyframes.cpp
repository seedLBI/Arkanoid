#include "Keyframes.h"


Keyframes::Keyframes() {
	default_value = 0.f;
}

Keyframes::Keyframes(const float& default_value) {
	this->default_value = default_value;
}

void Keyframes::setDefaultValue(const float& value) {
	this->default_value = value;
}


void Keyframes::discretize() {
	samples.clear();

	for (size_t i = 1; i < keys.size(); i++) {

		size_t first_step = i == 1 ? 0 : 1;
		const Keyframe k1 = keys[i - 1];
		const Keyframe k2 = keys[i];

		for (size_t j = first_step; j <= max_sampleRate; j++) {

			const float time = float(j) / (float)max_sampleRate;

			const glm::vec2 sample = lerp(k1, k2, time);

			samples.emplace_back(sample);

		}
	}
}

float Keyframes::evaluate(const float& time) const{
	if (samples.empty())
		return default_value;

	if (time <= samples.front().x)
		return samples.front().y;

	if (time >= samples.back().x)
		return samples.back().y;

	if (keys.size() == 1)
		return keys[0].pos.y;

	// binary search
	int left = 0;
	int right = samples.size() - 1;

	while (left < right) {
		int middle = left + (right - left + 1) / 2;

		if (time < samples[middle].x)
			right = middle - 1;
		else
			left = middle;
	}

	if (time == samples[left].x) {
		return samples[left].y;
	}

	float x0 = samples[left].x;
	float y0 = samples[left].y;
	float x1 = samples[left + 1].x;
	float y1 = samples[left + 1].y;

	float t = (time - x0) / (x1 - x0);
	return y0 + t * (y1 - y0);
}



void Keyframes::correctAllKeys() {
	if (keys.size() <= 1)
		return;

	for (size_t i = 0; i < keys.size() - 1; i++) {

		if (keys[i].out.x > keys[i + 1].pos.x)
			keys[i].out.x = keys[i + 1].pos.x;

		if (keys[i+1].in.x < keys[i].pos.x)
			keys[i + 1].in.x = keys[i].pos.x;
	}

}


void Keyframes::addPoint(const glm::vec2& p) {
	Keyframe temp;
	temp.pos = p;
	temp.in = p - glm::vec2(1.f, 0.f);
	temp.out = p + glm::vec2(1.f, 0.f);

	if (keys.empty()) {
		keys.emplace_back(temp);
	}
	else if (p.x < keys.front().pos.x) {
		float half_path = (keys.front().pos.x - p.x) / 2.f;

		temp.in  = p - glm::vec2{ half_path,0.f };
		temp.out = p + glm::vec2{ half_path,0.f };

		keys.insert(keys.begin(), temp);
	}
	else if (p.x > keys.back().pos.x) {
		float half_path = (p.x - keys.back().pos.x) / 2.f;

		temp.in = p - glm::vec2{ half_path,0.f };
		temp.out = p + glm::vec2{ half_path,0.f };

		keys.emplace_back(temp);
	}
	else {
		int index_candidate = 0;

		while (true) {
			if (p.x > keys[index_candidate].pos.x) {
				index_candidate++;
				continue;
			}
			else {
				index_candidate--;

				Keyframe& left = keys[index_candidate];
				Keyframe& right = keys[index_candidate + 1];

				float left_half_path  = (p.x - left.pos.x) / 2.f;
				float right_half_path = (right.pos.x - p.x) / 2.f;

				temp.in = p - glm::vec2{ left_half_path, 0.f };
				temp.out = p + glm::vec2{ right_half_path, 0.f };

				left.out = left.pos + glm::vec2{ left_half_path, 0.f };
				right.in = right.pos - glm::vec2{ right_half_path, 0.f };

				keys.insert(keys.begin() + index_candidate, temp);

				break;
			}


		}

	}
	

	correctAllKeys();

	discretize();
}

void Keyframes::addDeltaTime(const float& time) {
	for (Keyframe& key : keys) {
		key.in += time;
		key.out += time;
		key.pos += time;
	}

	discretize();
}

float Keyframes::getBeginTime() {
	return keys.front().pos.x;
}

float Keyframes::getEndTime() {
	return keys.back().pos.y;
}

glm::vec2 Keyframes::lerp(const Keyframe& k1, const Keyframe& k2, const float& t) {
	const glm::vec2 p0 = k1.pos;
	const glm::vec2 p1 = k1.out;
	const glm::vec2 p2 = k2.in;
	const glm::vec2 p3 = k2.pos;

	auto lerp_func = [](const glm::vec2& a, const glm::vec2& b, const float& time) {
		return a + time * (b - a);
		};

	glm::vec2 a = lerp_func(p0, p1, t);
	glm::vec2 b = lerp_func(p1, p2, t);
	glm::vec2 c = lerp_func(p2, p3, t);

	glm::vec2 d = lerp_func(a, b, t);
	glm::vec2 e = lerp_func(b, c, t);

	return lerp_func(d, e, t);
}

nlohmann::json Keyframes::Save() {
	nlohmann::json output;

	for (size_t i = 0; i < keys.size(); i++) {

		output[i]["in"]["x"] = keys[i].in.x;
		output[i]["in"]["y"] = keys[i].in.y;

		output[i]["pos"]["x"] = keys[i].pos.x;
		output[i]["pos"]["y"] = keys[i].pos.y;

		output[i]["out"]["x"] = keys[i].out.x;
		output[i]["out"]["y"] = keys[i].out.y;

	}

	return output;
}

void Keyframes::Load(const nlohmann::json& data) {

	keys.clear();

	keys.resize(data.size());

	for (const auto& [key, data] : data.items()) {

		size_t index = std::stoi(key);

		Keyframe& temp = keys[index];

		temp.in.x = data["in"]["x"].get<float>();
		temp.in.y = data["in"]["y"].get<float>();

		temp.pos.x = data["pos"]["x"].get<float>();
		temp.pos.y = data["pos"]["y"].get<float>();

		temp.out.x = data["out"]["x"].get<float>();
		temp.out.y = data["out"]["y"].get<float>();
	}

	discretize();
}