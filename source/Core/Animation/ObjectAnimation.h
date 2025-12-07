#ifndef OBJECT_ANIMATION_H
#define OBJECT_ANIMATION_H

#include <string>
#include <robin_hood.h>
#include "Core/Animation/Keyframes/Keyframes.h"

struct ObjectAnimation {
	std::string name;
	Keyframes value;
};

struct Animation {
	std::string name;
	robin_hood::unordered_flat_map<std::string, Keyframes> anims;
	bool repeat = false;
};

struct Animations {
	robin_hood::unordered_flat_map<std::string, Animation> animation;
};

#endif