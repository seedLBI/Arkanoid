#pragma once

#include "Core/Engine/Engine.h"

#include "Utils/OpenGL/Shaders/Shader.h"
#include "Utils/OpenGL/AtlasTexture/AtlasTexture.h"

#include <glm/glm.hpp>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class QuadInstancedTextured {
private:
	struct InstanceData {
		glm::vec4 pos_and_size;
		glm::vec4 tangent_and_normal;
		glm::vec4 uv_min_and_size;
		glm::vec4 cols_rows_animIndex_alpha;
		glm::vec3 tint;
		GLfloat isGrayScale;
		GLfloat layer;
	};
public:
	QuadInstancedTextured();
	~QuadInstancedTextured();

	void Add(
		const glm::vec2& position,
		const glm::vec2& size,
		const glm::vec2& tangent,
		const glm::vec2& normal,
		const AtlasObject& data,
		const float& alpha = 1.f, 
		const float& AnimIndex = 0.f);

	void Add(
		const glm::vec2& position,
		const glm::vec2& size,
		const glm::vec2& tangent,
		const glm::vec2& normal,
		const AtlasObject& data,
		const glm::vec3 tint,
		const float& alpha = 1.f,
		const float& AnimIndex = 0.f);

	void InitBindTexture(const TextureActiveNumber& bindingNumber);
	void Render();

protected:
	GLuint VAO, VBO, VBOinstanced, EBO;
	Shader* shader;
	const unsigned long long MAX_INSTANCES = 1024;
	std::vector<InstanceData> instances;
	
	void Init();
};
