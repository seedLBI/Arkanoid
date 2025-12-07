#pragma once

#include "Utils/OpenGL/Shaders/Shader.h"
#include "Utils/OpenGL/Framebuffer/Framebuffer.h"

#include <glm/glm.hpp>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct DebugCircleData {
	float thikness = 1.f; // from 0.f to 1.f
	glm::vec4 color;
	glm::mat4 model;
	DebugCircleData() = default;
	DebugCircleData(const glm::vec2& pos, const float& radius, const glm::vec4& color) {
		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(pos, 0.f));
		model = glm::scale(model, glm::vec3(radius, radius, 1.f));
		this->color = color;
		this->thikness = 1.f;
	}

	DebugCircleData(const glm::vec2& pos, const float& radius,const float& thikness, const glm::vec4& color) {
		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(pos, 0.f));
		model = glm::scale(model, glm::vec3(radius, radius, 1.f));
		this->color = color;
		this->thikness = thikness;
	}

};

class DebugCircle{
public:
	DebugCircle();
	~DebugCircle();


	void Render();

	void Add(const glm::vec2& pos, const float& radius,const glm::vec4& color, glm::vec2(*func_transform)(const glm::vec2&) = nullptr);
	void Add(const glm::vec2& pos, const float& radius, const float& thikness, const glm::vec4& color, glm::vec2(*func_transform)(const glm::vec2&) = nullptr);

	bool IsCollision(const glm::vec2& pos, const float& radius, const glm::vec2& point);
private:
	std::vector<DebugCircleData> data;
	GLuint VAO, VBO, EBO, VBO_instanced;
	Shader* shader;
	Shader* shader_generate;

	int last_size = 0;

	void Init();
	void RenderTexture();
	Framebuffer framebuffer;
	Texture texture;
};


