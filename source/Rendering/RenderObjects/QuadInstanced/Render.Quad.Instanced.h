#ifndef RENDER_QUAD_INSTANCED_H
#define RENDER_QUAD_INSTANCED_H

#include "Core/Engine/Engine.h"
#include "Utils/OpenGL/Shaders/Shader.h"

#include <glm/glm.hpp>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class QuadInstanced {
private:
	struct InstanceData {
		glm::vec4 pos_and_size;
		glm::vec4 tangent_and_normal;
		glm::vec4 color;
	};
public:
	QuadInstanced();
	~QuadInstanced();

	void Render();

	void AddLine(
		const glm::vec2& begin,
		const glm::vec2& end,
		const glm::vec4& color = glm::vec4(1.f),
		glm::vec2(*func_transform)(const glm::vec2&) = nullptr);

	void AddLine(
		const glm::vec2& begin,
		const glm::vec2& end,
		const float& thikness,
		const glm::vec4& color = glm::vec4(1.f),
		glm::vec2(*func_transform)(const glm::vec2&) = nullptr);

	void AddRectangle(
		const glm::vec2& position,
		const glm::vec2& size,
		const glm::vec4& color = glm::vec4(1.f),
		glm::vec2(*func_transform)(const glm::vec2&) = nullptr);

	void AddRectangleLines(
		const glm::vec2& min,
		const glm::vec2& max,
		const float& thikness,
		const glm::vec4& color = glm::vec4(1.f),
		glm::vec2(*func_transform)(const glm::vec2&) = nullptr);


	void AddRectangleRotated(
		const glm::vec2& position,
		const glm::vec2& size,
		const glm::vec2& normal,
		const glm::vec2& tangent,
		const glm::vec4& color = glm::vec4(1.f),
		glm::vec2(*func_transform)(const glm::vec2&) = nullptr);



protected:
	GLuint VAO, VBO, VBOinstanced, EBO;
	Shader* shader;
#ifndef _DEBUG
	const unsigned long long MAX_INSTANCES = 4096;
#endif // !_DEBUG

#ifdef _DEBUG
	const unsigned long long MAX_INSTANCES = 8192*4;
#endif // _DEBUG

	std::vector<InstanceData> instances;

	void Init();
};




#endif