#include "BallSpawnPosition.h"



nlohmann::json BallSpawnPosition::Save() {
	nlohmann::json output;

	output["globalPos"]["x"] = globalPos.x;
	output["globalPos"]["y"] = globalPos.y;

	output["tangent"]["x"] = tangent.x;
	output["tangent"]["y"] = tangent.y;

	output["global_radius"] = global_radius;


	return output;
}

void BallSpawnPosition::Load(const nlohmann::json& data) {
	if (data.contains("globalPos")) {
		globalPos.x = data["globalPos"]["x"].get<float>();
		globalPos.y = data["globalPos"]["y"].get<float>();
	}
	else
		globalPos = { 0.f,0.f };

	if (data.contains("tangent")) {
		tangent.x = data["tangent"]["x"].get<float>();
		tangent.y = data["tangent"]["y"].get<float>();
	}
	else
		tangent = { 0.f,0.1f };


	if (data.contains("global_radius"))
		global_radius = data["global_radius"].get<float>();
	else
		global_radius = 0.05f;

}