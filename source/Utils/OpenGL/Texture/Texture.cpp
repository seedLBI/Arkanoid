#include "Texture.h"


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#ifdef _DEBUG
#include <iostream>
#endif

#include "Utils/File/File.h"

Texture::Texture() {
	glGenTextures(1, &ID);
#ifdef _DEBUG
	std::cout << "Texture::Texture()\n";
#endif
}

Texture::~Texture() {
	if (dataImage)
		stbi_image_free(dataImage);
	glDeleteTextures(1, &ID);
#ifdef _DEBUG
	std::cout << "Texture::~Texture()\n";
#endif
}

Texture::Texture(const Texture2DSetting& setting, const std::string& Path2Image, const int& desiredCount_channels) : Texture() {
	SetSetting(setting);
	LoadTexture(Path2Image, desiredCount_channels);
	Init();
}

Texture::Texture(const Texture2DSetting& setting, const TextureDataSetting& setting_data, const void* data = nullptr) : Texture() {
	SetSetting(setting);
	SetDataSetting(setting_data);
	Init();
}


int Texture::GetWidth() {
	return setting_data.width;
}
int Texture::GetHeight() {
	return setting_data.height;
}

int Texture::GetID() {
	return ID;
}

void Texture::SetSetting(const Texture2DSetting& setting) {
#ifdef _DEBUG
	std::cout << "void Texture::SetSetting(const Texture2DSetting& setting)\n";
#endif
	this->setting = setting;
}

void Texture::SetDataSetting(const TextureDataSetting& setting_data) {
#ifdef _DEBUG
	std::cout << "void Texture::SetDataSetting(const TextureDataSetting& setting_data)\n";
#endif
	this->setting_data = setting_data;
}


std::pair<TexturePixelFormat, TextureInternalPixelFormat> Texture::GetTypeImageData(const int& channels) {
	switch (channels) {
	case 4:
		return { TexturePixelFormat::RGBA, TextureInternalPixelFormat::RGBA8 };
		break;
	case 3:
		return { TexturePixelFormat::RGB, TextureInternalPixelFormat::RGB8 };
		break;
	case 2:
		return { TexturePixelFormat::RG, TextureInternalPixelFormat::RG8 };
		break;
	case 1:
		return { TexturePixelFormat::RED, TextureInternalPixelFormat::R8 };
		break;
	case 0:
		exit(100);
		break;
	default:
		break;
	}
}

void Texture::LoadTexture(const std::string& Path2Image, const int& desiredCount_channels) {
#ifdef _DEBUG
	std::cout << "void Texture::LoadTexture(const std::string& Path2Image, const int& desiredCount_channels)" << std::endl;
	std::cout << "Path: [" << Path2Image << "]" << std::endl;
#endif

	if (!isFileExist(Path2Image)) {
		throw std::runtime_error("Incorrect path to image: [" + Path2Image + "]");
	}

	stbi_set_flip_vertically_on_load(true);

	int width, height, count_channels;
	dataImage = stbi_load(Path2Image.c_str(), &width, &height, &count_channels, desiredCount_channels);


	auto TypeData = GetTypeImageData(count_channels);

	setting_data.width			= width;
	setting_data.height			= height;
	setting_data.MipMapLevel	= 0;
	setting_data.format			= TypeData.first;
	setting_data.internalFormat = TypeData.second;
	setting_data.type			= TextureDataType::_UNSIGNED_BYTE;
	

#ifdef _DEBUG
	std::cout << "[Texture] [" << Path2Image << "]\n";
	std::cout << "\t\tWidth:   [" << setting_data.width << "]\n";
	std::cout << "\t\tHeight:  [" << setting_data.height << "]\n";
	std::cout << "\t\tChannels:[" << count_channels << "]\n";
#endif


}
void Texture::GenerateMipMap() {
	glGenerateMipmap(GL_TEXTURE_2D);
}


void Texture::Bind() {
	glBindTexture(GL_TEXTURE_2D, ID);
}
void Texture::SetActive(const TextureActiveNumber& number) {
	glActiveTexture(number);
}


void Texture::Init() {
#ifdef _DEBUG
	std::cout << "Texture::Init()\n";
#endif
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, setting.WrapX);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, setting.WrapY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, setting.Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, setting.Max);

	const int mipmapFilters[4] = {
		GL_NEAREST_MIPMAP_NEAREST,
		GL_NEAREST_MIPMAP_LINEAR,
		GL_LINEAR_MIPMAP_NEAREST,
		GL_LINEAR_MIPMAP_LINEAR };
	bool haveMipmapFilter = false;
	for (int i = 0; i < 4; i++)
	{
		if (setting.Min == mipmapFilters[i]
			|| setting.Max == mipmapFilters[i])
		{
			haveMipmapFilter = true;
			break;
		}
	}


	glTexImage2D(
		GL_TEXTURE_2D,
		setting_data.MipMapLevel,
		setting_data.internalFormat,
		setting_data.width,
		setting_data.height,
		0,
		setting_data.format,
		setting_data.type,
		dataImage);


	if (haveMipmapFilter) {
		GenerateMipMap();
	}

	if (dataImage) {
		stbi_image_free(dataImage);
		dataImage = nullptr;
	}

}
