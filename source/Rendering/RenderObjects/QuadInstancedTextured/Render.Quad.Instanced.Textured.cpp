#include "Render.Quad.Instanced.Textured.h"

QuadInstancedTextured::QuadInstancedTextured() {
	Init();

}

QuadInstancedTextured::~QuadInstancedTextured() {
	if (shader) { delete shader; shader = nullptr; }
	if (VBOinstanced) glDeleteBuffers(1, &VBOinstanced);
	if (VBO) glDeleteBuffers(1, &VBO);
	if (EBO) glDeleteBuffers(1, &EBO);
	if (VAO) glDeleteVertexArrays(1, &VAO);
}

void QuadInstancedTextured::Add(
	const glm::vec2& position,
	const glm::vec2& size,
	const glm::vec2& tangent,
	const glm::vec2& normal,
	const AtlasObject& data,
	const float& alpha,
	const float& AnimIndex)
{
	InstanceData instance;

	instance.pos_and_size	= glm::vec4(
		position, 
		size);

	instance.tangent_and_normal	= glm::vec4(
		tangent, 
		normal);

	instance.uv_min_and_size = glm::vec4(
		data.uv_min, 
		data.uv_size);

	instance.cols_rows_animIndex_alpha = glm::vec4(
		data.cols, 
		data.rows, 
		std::fmod(AnimIndex, (float)(data.cols * data.rows)),
		alpha);

	instance.tint = glm::vec3(1.0f);
	instance.isGrayScale = 0.0f;
	instance.layer = (float)data.index_layer;

	instances.emplace_back(instance);
}

void QuadInstancedTextured::Add(
	const glm::vec2& position,
	const glm::vec2& size,
	const glm::vec2& tangent,
	const glm::vec2& normal,
	const AtlasObject& data,
	const glm::vec3 tint,
	const float& alpha,
	const float& AnimIndex) {

	InstanceData instance;

	instance.pos_and_size = glm::vec4(
		position, 
		size);

	instance.tangent_and_normal = glm::vec4(
		tangent, 
		normal);

	instance.uv_min_and_size = glm::vec4(
		data.uv_min, 
		data.uv_size);

	instance.cols_rows_animIndex_alpha = glm::vec4(
		data.cols, 
		data.rows, 
		std::fmod(AnimIndex, (float)(data.cols* data.rows)),
		alpha);

	instance.isGrayScale = 1.f;
	instance.tint = tint;
	instance.layer = (float)data.index_layer;

	instances.emplace_back(instance);
}




void QuadInstancedTextured::Render() {
	if (instances.empty()) return;

	if (instances.size() > MAX_INSTANCES) { instances.resize(MAX_INSTANCES); }
	glBindBuffer(GL_ARRAY_BUFFER, VBOinstanced);
	glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * sizeof(InstanceData), NULL, GL_DYNAMIC_DRAW); // Orphan with fixed size.
	glBufferSubData(GL_ARRAY_BUFFER, 0, instances.size() * sizeof(InstanceData), instances.data());



	shader->use();
	glBindVertexArray(VAO);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, static_cast<GLsizei>(instances.size())); // instances.size() Ч кол-во спрайтов
	glBindVertexArray(0);

	instances.clear();
}

void QuadInstancedTextured::InitBindTexture(const TextureActiveNumber& bindingNumber) {
	shader->use();
	shader->setInt("atlasTexture", (int)bindingNumber);
}


void QuadInstancedTextured::Init() {
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(InstanceData), nullptr, GL_DYNAMIC_DRAW);


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
	offset += sizeof(float) * 4;

	//  (location=5)
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offset);
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(5, 1);
	offset += sizeof(float) * 4;

	//  (location=6)
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offset);
	glEnableVertexAttribArray(6);
	glVertexAttribDivisor(6, 1);
	offset += sizeof(float) * 3;

	//  (location=7)
	glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offset);
	glEnableVertexAttribArray(7);
	glVertexAttribDivisor(7, 1);
	offset += sizeof(float) * 1;

	//  (location=8)
	glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offset);
	glEnableVertexAttribArray(8);
	glVertexAttribDivisor(8, 1);


	glBindVertexArray(0);


	shader = new Shader(
		"InstanceTexturedQuad_Shader",
		(std::string)"resources/shaders/QuadInstancedTexture/QuadInstancedTexture.vert",
		(std::string)"resources/shaders/QuadInstancedTexture/QuadInstancedTexture.frag");

	shader->use();

	engine::shader::BindUBO_MainBlock(*shader);
}
