#ifndef TEXTURE_H
#define TEXTURE_H


#define GLEW_STATIC
#include <GL/glew.h>

#ifdef _WIN32
	#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <string>
#include <stdexcept>

#include "Utils/File/File.h"
#include "Utils/OpenGL/Enums/OpenGL.Enums.h"
#include "Utils/OpenGL/TextureStructs/TextureStructs.h"



class Texture {
public:
	Texture();
	Texture(const Texture2DSetting& setting, const TextureDataSetting& setting_data, const void* data);
	Texture(const Texture2DSetting& setting, const std::string& Path2Image, const int& desiredCount_channels = 0);
	~Texture();

	void SetSetting(const Texture2DSetting& setting);
	void SetDataSetting(const TextureDataSetting& setting);
	void LoadTexture(const std::string& Path2Image,const int& desiredCount_channels = 0);
	void GenerateMipMap();
	void Init();


	void Bind();
	void SetActive(const TextureActiveNumber& number);

	int GetWidth();
	int GetHeight();

	int GetID();
private:
	GLuint ID;

	Texture2DSetting setting;
	TextureDataSetting setting_data;


	uint8_t* dataImage = nullptr;
	std::pair<TexturePixelFormat, TextureInternalPixelFormat> GetTypeImageData(const int& channels);
};







#endif