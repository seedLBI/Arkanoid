#pragma once

#include "Rendering/RenderObjects/Quad/Render.Quad.h"
#include "Utils/OpenGL/Shaders/Shader.h"
#include "Utils/OpenGL/Texture/Texture.h"

#include <glm/glm.hpp>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class QuadTexture {
public:
	QuadTexture();
	~QuadTexture();

	void DrawFullScreenByAspectRatio(Texture& texture);

protected:
	Quad quad;
	Shader* shader;
	void Init();
};


