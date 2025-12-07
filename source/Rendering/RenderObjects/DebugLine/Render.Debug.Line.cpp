#include "Render.Debug.Line.h"
#include "Core/Engine/Engine.h"

DebugLine::DebugLine() {
	Init();
}
DebugLine::~DebugLine() {

}

void DebugLine::Init() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	float a_t[2] = { 0.f,1.f };
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(float), a_t, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);

	glGenBuffers(1, &VBO_instanced);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_instanced);

	GLsizei stride = sizeof(DebugLineData);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(0));
	glVertexAttribDivisor(1, 1);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 2));
	glVertexAttribDivisor(2, 1);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 4));
	glVertexAttribDivisor(3, 1);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 8));
	glVertexAttribDivisor(4, 1);

	glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_DYNAMIC_DRAW);


	shader = new Shader("LineInstanced_Shader",
				(std::string)"resources/shaders/Debug/Line/DebugLine.vert",
				(std::string)"resources/shaders/Debug/Line/DebugLine.geom",
				(std::string)"resources/shaders/Debug/Line/DebugLine.frag");


	shader->use();

	engine::shader::BindUBO_MainBlock(*shader);
}

void DebugLine::Render() {
	if (data.empty()) return;

	glBindBuffer(GL_ARRAY_BUFFER, VBO_instanced);

	if (data.size() == last_size)
		glBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(DebugLineData), data.data());
	else
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(DebugLineData), data.data(), GL_DYNAMIC_DRAW);

	shader->use();

	glBindVertexArray(VAO);
	glDrawArraysInstanced(GL_LINES, 0, 2, (GLsizei)data.size());
	last_size = data.size();
	data.clear();
}

void DebugLine::Update() {

}

void DebugLine::Add(
	const glm::vec2& begin, 
	const glm::vec2& end, 
	const glm::vec4& color, 
	glm::vec2(*func_transform)(const glm::vec2&)) 
{
	if (func_transform == nullptr)
		data.emplace_back(DebugLineData{ begin, end, color, 5.f });
	else {
		data.emplace_back(DebugLineData{ func_transform(begin), func_transform(end), color, 5.f });
	}
}

void DebugLine::AddRectangle(
	const glm::vec2& position,
	const glm::vec2& size,
	const glm::vec2& normal,
	const glm::vec2& tangent,
	const glm::vec4& color,
	glm::vec2(*func_transform)(const glm::vec2&))
{

	glm::vec2 p_s = position;
	glm::vec2 n_s = normal * size.x;
	glm::vec2 t_s = tangent * size.y;

	const glm::vec2 v1 = p_s + n_s + t_s;
	const glm::vec2 v2 = p_s - n_s + t_s;
	const glm::vec2 v3 = p_s - n_s - t_s;
	const glm::vec2 v4 = p_s + n_s - t_s;


	data.emplace_back(DebugLineData{ v1, v2, color, 5.f });
	data.emplace_back(DebugLineData{ v2, v3, color, 5.f });
	data.emplace_back(DebugLineData{ v3, v4, color, 5.f });
	data.emplace_back(DebugLineData{ v4, v1, color, 5.f });
	data.emplace_back(DebugLineData{ v1, v3, color, 5.f });

}


void DebugLine::Add(
	const glm::vec2& begin,
	const glm::vec2& end,
	const float& thikness,
	const glm::vec4& color,
	glm::vec2(*func_transform)(const glm::vec2&)) {

	if (func_transform == nullptr)
		data.emplace_back(DebugLineData{ begin, end, color, thikness });
	else {
		data.emplace_back(DebugLineData{ func_transform(begin), func_transform(end), color, thikness });
	}
}
