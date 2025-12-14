#include "Render.Triangle.Instanced.h"

TriangleInstanced::TriangleInstanced() {
	Init();
}

TriangleInstanced::~TriangleInstanced() {
	if (shader) { delete shader; shader = nullptr; }
	if (VBOinstanced) glDeleteBuffers(1, &VBOinstanced);
	if (VBO) glDeleteBuffers(1, &VBO);
	if (VAO) glDeleteVertexArrays(1, &VAO);
}

void TriangleInstanced::Render() {
	if (instances.empty()) return;

	if (instances.size() > MAX_INSTANCES) { instances.resize(MAX_INSTANCES); }
	glBindBuffer(GL_ARRAY_BUFFER, VBOinstanced);
	glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * sizeof(InstanceData), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, instances.size() * sizeof(InstanceData), instances.data());

	shader->use();
	glBindVertexArray(VAO);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 3, static_cast<GLsizei>(instances.size()));
	glBindVertexArray(0);

	instances.clear();
}

void TriangleInstanced::Add(
	const glm::vec2& p1,
	const glm::vec2& p2,
	const glm::vec2& p3,
	const glm::vec4& color,
	glm::vec2(*func_transform)(const glm::vec2&)) {

	InstanceData temp;
	if (func_transform != nullptr) {
		temp.p1 = func_transform(p1);
		temp.p2 = func_transform(p2);
		temp.p3 = func_transform(p3);
	}
	else {
		temp.p1 = p1;
		temp.p2 = p2;
		temp.p3 = p3;
	}

	temp.color = color;



	instances.push_back(temp);
}


void TriangleInstanced::Init() {
	GLfloat vertices[] = { 
		0.f, 0.f, 
		0.f, 0.f, 
		0.f, 0.f, };

	GLuint indexes[] = {
		0, 1, 2
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)(sizeof(float) * 0)); // Position
	glEnableVertexAttribArray(0);


	glGenBuffers(1, &VBOinstanced);
	glBindBuffer(GL_ARRAY_BUFFER, VBOinstanced);
	glBufferData(GL_ARRAY_BUFFER, sizeof(InstanceData) * 1000, nullptr, GL_DYNAMIC_DRAW);

	size_t offset = 0;

	//(location=2)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offset);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);
	offset += sizeof(float) * 2;

	//(location=3)
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offset);
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);
	offset += sizeof(float) * 2;

	//(location=4)
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offset);
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, 1);
	offset += sizeof(float) * 2;

	//(location=5)
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offset);
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(5, 1);


	glBindVertexArray(0);

	shader = new Shader(
		"InstanceTriangle_Shader",
		(std::string)"resources/shaders/TriangleInstanced/TriangleInstanced.vert",
		(std::string)"resources/shaders/TriangleInstanced/TriangleInstanced.frag");

	shader->use();


	engine::shader::BindUBO_MainBlock(*shader);
}