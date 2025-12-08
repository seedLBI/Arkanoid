#ifndef COORDINATE_SYSTEM_H
#define COORDINATE_SYSTEM_H

#include <glm/glm.hpp>

glm::vec2 TranslateGlobalToScreen(const glm::vec2& global);
glm::vec2 TranslateScreenToGlobal(const glm::vec2& screen);

float TranslateScalar_GlobalToScreen(const float& value);
float TranslateScalar_ScreenToGlobal(const float& value);

float ref_scale();


#endif
