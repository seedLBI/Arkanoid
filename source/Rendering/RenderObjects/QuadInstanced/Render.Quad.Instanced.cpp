#include "Render.Quad.Instanced.h"

#include "Game/Math/Arkanoid_Math.h"
#include "Core/CoordinateSystem/CoordinateSystem.h"


QuadInstanced::QuadInstanced() {
	Init();
}
QuadInstanced::~QuadInstanced() {
	if (shader) { delete shader; shader = nullptr; }
	if (VBOinstanced) glDeleteBuffers(1, &VBOinstanced);
	if (VBO) glDeleteBuffers(1, &VBO);
	if (EBO) glDeleteBuffers(1, &EBO);
	if (VAO) glDeleteVertexArrays(1, &VAO);
}

void QuadInstanced::Render() {
	if (instances.empty()) return;

	if (instances.size() > MAX_INSTANCES) { instances.resize(MAX_INSTANCES); }
	glBindBuffer(GL_ARRAY_BUFFER, VBOinstanced);
	glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * sizeof(InstanceData), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, instances.size() * sizeof(InstanceData), instances.data());

	shader->use();
	glBindVertexArray(VAO);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, static_cast<GLsizei>(instances.size()));
	glBindVertexArray(0);

	instances.clear();
}

void QuadInstanced::AddLine(
	const glm::vec2& begin,
	const glm::vec2& end,
	const glm::vec4& color,
	glm::vec2(*func_transform)(const glm::vec2&)) 
{
	AddLine(begin, end, 5.f, color, func_transform);
}

void QuadInstanced::AddLine(
	const glm::vec2& begin,
	const glm::vec2& end,
	const float& thikness,
	const glm::vec4& color,
	glm::vec2(*func_transform)(const glm::vec2&)) 
{
	glm::vec2 dir = end - begin;
	glm::vec2 pos = (begin + end) / 2.f;

	glm::vec2 normal = -perp_normalized(dir);
	glm::vec2 tangent = glm::normalize(dir);

	glm::vec2 size_global = glm::vec2( TranslateScalar_GlobalToScreen(glm::length(dir)), thikness);

	AddRectangleRotated(pos, size_global, normal, tangent, color, func_transform);

}

void QuadInstanced::AddRectangleRotated(
	const glm::vec2& position,
	const glm::vec2& size,
	const glm::vec2& normal,
	const glm::vec2& tangent,
	const glm::vec4& color,
	glm::vec2(*func_transform)(const glm::vec2&)) 
{
	InstanceData temp;
	temp.tangent_and_normal = glm::vec4(tangent, normal);
	temp.color = color;



	if (func_transform != nullptr)
		temp.pos_and_size = glm::vec4{ func_transform(position), size};
	else {
		temp.pos_and_size = glm::vec4(position, size);
	}

	temp.tangent_and_normal = glm::vec4{ tangent,normal };
	temp.color = color;

	instances.push_back(temp);
}

void QuadInstanced::AddRectangleLines(
	const glm::vec2& min,
	const glm::vec2& max,
	const float& thikness,
	const glm::vec4& color,
	glm::vec2(*func_transform)(const glm::vec2&)) {

	glm::vec2 v1 = min;
	glm::vec2 v2 = { min.x, max.y };
	glm::vec2 v3 = max;
	glm::vec2 v4 = { max.x,min.y };

	AddLine(v1, v2, thikness, color, func_transform);
	AddLine(v2, v3, thikness, color, func_transform);
	AddLine(v3, v4, thikness, color, func_transform);
	AddLine(v4, v1, thikness, color, func_transform);
}

void QuadInstanced::AddRectangle(
	const glm::vec2& position,
	const glm::vec2& size,
	const glm::vec4& color,
	glm::vec2(*func_transform)(const glm::vec2&))
{
	InstanceData temp;

	if (func_transform != nullptr)
		temp.pos_and_size = glm::vec4{ func_transform(position), 2.f * size };
	else
		temp.pos_and_size = glm::vec4{ position,size };

	glm::vec2 dir = glm::vec2{ 0.f,1.f };

	temp.tangent_and_normal = glm::vec4{ perp_normalized(dir), dir,  };
	temp.color = color;

	instances.push_back(temp);


}


void QuadInstanced::Init() {
	GLfloat vertices[] = {
		-0.5f, -0.5f,  0.f,  0.f, // 0
		 0.5f, -0.5f,  1.f,  0.f, // 1
		 0.5f,  0.5f,  1.f,  1.f, // 2
		-0.5f,  0.5f,  0.f,  1.f, // 3
	};

	GLuint indexes[] = {
		0, 1, 2, 2, 3, 0
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 0)); // Position
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2)); // TexCoord
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);




	glGenBuffers(1, &VBOinstanced);
	glBindBuffer(GL_ARRAY_BUFFER, VBOinstanced);
	glBufferData(GL_ARRAY_BUFFER, sizeof(InstanceData) * 1000, nullptr, GL_DYNAMIC_DRAW);

	size_t offset = 0;

	//(location=2)
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offset);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);
	offset += sizeof(float) * 4;

	//(location=3)
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offset);
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);
	offset += sizeof(float) * 4;

	//(location=4)
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offset);
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, 1);
	//offset += sizeof(float) * 4;

	glBindVertexArray(0);

	shader = new Shader(
		"InstanceQuad_Shader",
		(std::string)"resources/shaders/QuadInstanced/QuadInstanced.vert",
		(std::string)"resources/shaders/QuadInstanced/QuadInstanced.frag");

	shader->use();


	engine::shader::BindUBO_MainBlock(*shader);
}