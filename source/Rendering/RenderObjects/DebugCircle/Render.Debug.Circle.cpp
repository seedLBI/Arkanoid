#include "Render.Debug.Circle.h"
#include "Rendering/RenderObjects/Quad/Render.Quad.h"
#include "Core/Engine/Engine.h"


DebugCircle::DebugCircle() {
	Init();
	RenderTexture();
}
DebugCircle::~DebugCircle() {

}


void DebugCircle::Render() {
	if (data.empty()) return;

	glBindBuffer(GL_ARRAY_BUFFER, VBO_instanced);

	if (data.size() == last_size) {
		glBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(DebugCircleData), data.data());
	}
	else
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(DebugCircleData), data.data(), GL_DYNAMIC_DRAW);


	texture.SetActive(TextureActiveNumber::TEXTURE_0);
	texture.Bind();

	shader->use();
	glBindVertexArray(VAO);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, data.size());
	last_size = data.size();
	data.clear();

}


void DebugCircle::Add(const glm::vec2& pos, const float& radius, const glm::vec4& color, glm::vec2(*func_transform)(const glm::vec2&)) {
	if (func_transform == nullptr)
		this->data.emplace_back(DebugCircleData(pos, radius, color));
	else
		this->data.emplace_back(DebugCircleData(func_transform(pos), radius, color));
	
}

void DebugCircle::Add(const glm::vec2& pos, const float& radius, const float& thikness, const glm::vec4& color, glm::vec2(*func_transform)(const glm::vec2&) ) {
	if (func_transform == nullptr)
		this->data.emplace_back(DebugCircleData(pos, radius, thikness, color));
	else
		this->data.emplace_back(DebugCircleData(func_transform(pos), radius, thikness, color));
}

bool DebugCircle::IsCollision(const glm::vec2& pos, const float& radius, const glm::vec2& point) {
	return glm::distance(pos, point) <= radius;
}


void DebugCircle::Init() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);



	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	float vertex[8] = {
		-1.f,-1.f,
		-1.f, 1.f,
		1.f,1.f,
		1.f,-1.f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8.f, vertex, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);


	unsigned int indices[6]{
		0,1,2,
		0,3,2
	};

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO_instanced);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_instanced);

	GLsizei stride = sizeof(DebugCircleData);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 0));
	glVertexAttribDivisor(1, 1);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 1));
	glVertexAttribDivisor(2, 1);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 5));
	glVertexAttribDivisor(3, 1);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 9));
	glVertexAttribDivisor(4, 1);

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 13));
	glVertexAttribDivisor(5, 1);

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 17));
	glVertexAttribDivisor(6, 1);

	glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_DYNAMIC_DRAW);

	shader = new Shader("DebugCircleShader",
				(std::string)"resources/shaders/Debug/Circle/DebugCircle.vert",
				(std::string)"resources/shaders/Debug/Circle/DebugCircle.frag");

	shader->use();
	shader->setInt("u_sdf", 0);
	engine::shader::BindUBO_MainBlock(*shader);
}

void DebugCircle::RenderTexture() {

	const int sizeTexture = 32;

	Texture2DSetting setting;
	TextureDataSetting setting_data;

	setting.Min = TextureFilter::LINEAR;
	setting.Max = TextureFilter::LINEAR;
	setting.WrapX = TextureWrap::MIRRORED_REPEAT;
	setting.WrapY = TextureWrap::MIRRORED_REPEAT;

	setting_data.width			= sizeTexture;
	setting_data.height			= sizeTexture;
	setting_data.MipMapLevel	= 0;
	setting_data.internalFormat = TextureInternalPixelFormat::R32F;
	setting_data.format			= TexturePixelFormat::RED;
	setting_data.type			= TextureDataType::_FLOAT;


	texture.SetSetting(setting);
	texture.SetDataSetting(setting_data);
	texture.Init();

	framebuffer.AttachTexture(texture, Buffer_Attachment::COLOR_ATTACHMENT0);
	framebuffer.SetCountDrawBuffers(1);
	
	

	Quad quad;

	const char* vs = R"(
#version 330 core
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_tex;

out vec2 v_tex;

void main() {
    gl_Position = vec4(a_pos, 1.0);
    v_tex = a_tex;
}
)";


	const char* fs = R"(
#version 330 core
in vec2 v_tex;
out float OutValue;

uniform vec2  u_center = vec2(0.5,0.5);	// 0..1
uniform float u_radius = 0.48;			// 0..1
uniform float u_range  = 0.15;			// how much distance maps to 0.5 +/-


void main() {
	float d = length(v_tex - u_center) - u_radius; // in normalized UV
	float mapped = clamp(0.5 + d / u_range * 0.5, 0.0, 1.0);
	OutValue = 1.0 - mapped;
}
)";

	shader_generate = new Shader("SDF_Circle_Generator_Shader", vs, fs);


	glDisable(GL_BLEND);

	glViewport(0, 0, sizeTexture, sizeTexture);
	framebuffer.Bind();
		shader_generate->use();
		quad.Draw();
	framebuffer.UnBind();

	glm::vec2 s = engine::window::GetFramebufferSize();
	glViewport(0, 0, s.x, s.y);

	glEnable(GL_BLEND);


}
