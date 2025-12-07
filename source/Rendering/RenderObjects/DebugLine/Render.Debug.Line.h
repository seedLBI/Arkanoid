#pragma once

#include "Utils/OpenGL/Shaders/Shader.h"

#include <glm/glm.hpp>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>




struct DebugLineData {
	glm::vec2 begin;
	glm::vec2 end;
	glm::vec4 color;
	float thikness; // 0.f to infinity
};

class DebugLine {
public:
	DebugLine();
	~DebugLine();


	void Render();
	void Update();


	void Add(
		const glm::vec2& begin, 
		const glm::vec2& end, 
		const glm::vec4& color = glm::vec4(1.f),
		glm::vec2(*func_transform)(const glm::vec2&) = nullptr);


	void AddRectangle(
		const glm::vec2& position,
		const glm::vec2& size,
		const glm::vec2& normal,
		const glm::vec2& tangent,
		const glm::vec4& color = glm::vec4(1.f),
		glm::vec2(*func_transform)(const glm::vec2&) = nullptr );


	void Add(
		const glm::vec2& begin,
		const glm::vec2& end,
		const float& thikness,
		const glm::vec4& color = glm::vec4(1.f),
		glm::vec2(*func_transform)(const glm::vec2&) = nullptr);

private:
	std::vector<DebugLineData> data;
	GLuint VAO, VBO, VBO_instanced;
	Shader* shader;

	int last_size = 0;

	void Init();

};
