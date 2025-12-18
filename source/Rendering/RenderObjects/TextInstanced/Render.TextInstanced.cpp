#include "Render.TextInstanced.h"

TextInstanced::TextInstanced() {
	Init();
}

TextInstanced::~TextInstanced() {
	if (shader) { delete shader; shader = nullptr; }
	if (VBOinstanced) glDeleteBuffers(1, &VBOinstanced);
	if (VBO) glDeleteBuffers(1, &VBO);
	if (EBO) glDeleteBuffers(1, &EBO);
	if (VAO) glDeleteVertexArrays(1, &VAO);
}

void TextInstanced::AttachTexture(Texture& texture) {
	this->texture = &texture;
}

void TextInstanced::Render() {
	if (instances.empty()) return;

	if (instances.size() > MAX_INSTANCES) { instances.resize(MAX_INSTANCES); }
	glBindBuffer(GL_ARRAY_BUFFER, VBOinstanced);
	glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * sizeof(InstanceData), NULL, GL_DYNAMIC_DRAW); // Orphan with fixed size.
	glBufferSubData(GL_ARRAY_BUFFER, 0, instances.size() * sizeof(InstanceData), instances.data());


	texture->SetActive(TextureActiveNumber::TEXTURE_0);
	texture->Bind();
	shader->use();
	glBindVertexArray(VAO);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, static_cast<GLsizei>(instances.size())); // instances.size() Ч кол-во спрайтов
	glBindVertexArray(0);

	instances.clear();
}

void TextInstanced::Add(
	const glm::vec2& position,
	const glm::vec2& size,
	const glm::vec2& tangent,
	const glm::vec2& normal,
	const glm::vec2& uv_min,
	const glm::vec2& uv_size,
	const glm::vec4& color_base,
	const glm::vec4& color_outline
) {
	InstanceData new_object;

	new_object.color_base    = color_base;
	new_object.color_outline = color_outline;
	new_object.pos_and_size = { position,size };
	new_object.tangent_and_normal = { tangent,normal };
	new_object.uv_min_and_size = { uv_min, uv_size };
	new_object.thickness_base_and_outline = { 1.f,0.f };

	instances.push_back(new_object);
}


void TextInstanced::Init() {
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

	//(location=0)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 0)); // Position
	glEnableVertexAttribArray(0);

	//(location=1)
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
	offset += sizeof(float) * 4;

	//(location=5)
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offset);
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(5, 1);
	offset += sizeof(float) * 4;

	//(location=6)
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offset);
	glEnableVertexAttribArray(6);
	glVertexAttribDivisor(6, 1);
	offset += sizeof(float) * 4;

	//(location=7)
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offset);
	glEnableVertexAttribArray(6);
	glVertexAttribDivisor(6, 1);



	glBindVertexArray(0);

	shader = new Shader(
		"InstanceQuad_Shader",
		(std::string)"resources/shaders/TextInstanced/TextInstanced.vert",
		(std::string)"resources/shaders/TextInstanced/TextInstanced.frag");

	shader->use();
	shader->setInt("mtsdf_texture", 0);

	engine::shader::BindUBO_MainBlock(*shader);
}