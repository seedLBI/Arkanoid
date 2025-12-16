#ifndef RENDER_TRIANGLE_INSTANCED_H
#define RENDER_TRIANGLE_INSTANCED_H

#include "Core/Engine/Engine.h"
#include "Utils/OpenGL/Shaders/Shader.h"

#include <glm/glm.hpp>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include "Game/Math/Triangle/Triangle.h"

class TriangleInstanced {
private:
	struct InstanceData {
		glm::vec2 p1,p2,p3;
		glm::vec4 color;
	};
public:
	TriangleInstanced();
	~TriangleInstanced();

	void Render();

	void Add(
		const glm::vec2& p1,
		const glm::vec2& p2,
		const glm::vec2& p3,
		const glm::vec4& color = glm::vec4(1.f),
		glm::vec2(*func_transform)(const glm::vec2&) = nullptr);
	
	void Add(
		const Triangle& triangle,
		const glm::vec4& color = glm::vec4(1.f),
		glm::vec2(*func_transform)(const glm::vec2&) = nullptr);

private:
	GLuint VAO, VBO, VBOinstanced;
	Shader* shader;

	const unsigned long long MAX_INSTANCES = 4096;
	std::vector<InstanceData> instances;

	void Init();
};



#endif