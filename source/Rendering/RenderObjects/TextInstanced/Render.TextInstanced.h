#ifndef RENDER_TEXT_INSTANCED_H
#define RENDER_TEXT_INSTANCED_H

#include "Core/Engine/Engine.h"
#include "Utils/OpenGL/Shaders/Shader.h"
#include "Utils/OpenGL/Texture/Texture.h"

#include <glm/glm.hpp>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

 
class TextInstanced {
private:
	struct InstanceData {
		glm::vec4 pos_and_size;
		glm::vec4 tangent_and_normal;
		glm::vec4 uv_min_and_size;
		glm::vec4 color_base;
		glm::vec4 color_outline;
		glm::vec2 thickness_base_and_outline;
	};
public:
	TextInstanced();
	~TextInstanced();

	void Render();

	void AttachTexture(Texture& texture);

	void Add(
		const glm::vec2& position,
		const glm::vec2& size,
		const glm::vec2& tangent,
		const glm::vec2& normal,
		const glm::vec2& uv_min,
		const glm::vec2& uv_size,
		const glm::vec4& color_base,
		const glm::vec4& color_outline
	);

private:
	GLuint VAO, VBO, VBOinstanced, EBO;
	Shader* shader;
	Texture* texture;

	const unsigned long long MAX_INSTANCES = 4096;
	std::vector<InstanceData> instances;

	void Init();
};


#endif