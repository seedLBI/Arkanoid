#include "Texture2DArrays.h"

#include <stb_image.h>

Texture2DArrays::Texture2DArrays() {
	this->width = 0;
	this->height = 0;
	this->countLayers = 0;

	setting.WrapX = TextureWrap::CLAMP_TO_EDGE;
	setting.WrapY = TextureWrap::CLAMP_TO_EDGE;
	setting.Min = TextureFilter::LINEAR;
	setting.Max = TextureFilter::LINEAR;

}

Texture2DArrays::Texture2DArrays(const std::vector<std::string>& paths_to_images) {
	this->width = 0;
	this->height = 0;
	this->countLayers = 0;

	setting.WrapX = TextureWrap::CLAMP_TO_EDGE;
	setting.WrapY = TextureWrap::CLAMP_TO_EDGE;
	setting.Min = TextureFilter::LINEAR;
	setting.Max = TextureFilter::LINEAR;

	LoadImages(paths_to_images);
	Init();
}

Texture2DArrays::~Texture2DArrays() {
	for (auto p : images_data) {
		if (p) stbi_image_free(p);
	}
	images_data.clear();

	if (!IDs_layer.empty()) {
		glDeleteTextures(static_cast<GLsizei>(IDs_layer.size()), IDs_layer.data());
		IDs_layer.clear();
	}
	if (ID != 0) {
		glDeleteTextures(1, &ID);
		ID = 0;
	}

}

const int& Texture2DArrays::getWidth() {
	return width;
}
const int& Texture2DArrays::getHeight() {
	return height;
}
const int& Texture2DArrays::getCountLayers() {
	return countLayers;
}

void Texture2DArrays::Bind() {
	glBindTexture(GL_TEXTURE_2D_ARRAY, ID);
}
void Texture2DArrays::SetActive(const TextureActiveNumber& number) {
	glActiveTexture(number);
}
const GLuint& Texture2DArrays::GetID() {
	return ID;
}
const GLuint& Texture2DArrays::GetID_Layer(const int& layer_index) {
	return IDs_layer[layer_index];
}

void Texture2DArrays::LoadImages(const std::vector<std::string>& paths_to_images) {
	for (size_t i = 0; i < paths_to_images.size(); i++)
	{
		std::ifstream checkFile(paths_to_images[i]);
		if (!checkFile.good())
			assert("FILE NOT EXIST!!!");
		checkFile.close();
		uint8_t* data;
		int width, height, count_channels;
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load(paths_to_images[i].c_str(), &width, &height, &count_channels, 4);

#ifdef _DEBUG
		printf("[TEXTURE 2D ARRAYS][%i]\n\twidth: [%i]\n\theight: [%i]\n\tchannels: [%i]\n", int(i), width, height, count_channels);
#endif // _DEBUG

		if (i == 0) {
			this->width = width;
			this->height = height;
		}
		else if (width != this->width || height != this->height) {
			assert("Atlases size not equal!");
		}
		this->countLayers++;

		images_data.push_back(data);
	}
}


void Texture2DArrays::Init() {
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, ID);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, GLint(width), GLint(height), GLint(countLayers));

	for (size_t i = 0; i < images_data.size(); i++) {
		const GLint layer = (GLint)i;
		const uint8_t* data = images_data[i];

		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, setting.WrapX);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, setting.WrapY);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, setting.Min);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, setting.Max);

	IDs_layer.resize(countLayers);
	glGenTextures(countLayers, IDs_layer.data());
	

	for (int32_t i = 0; i < countLayers; ++i) {
		 glTextureView(IDs_layer[i], GL_TEXTURE_2D, ID, GL_RGBA8, 0, 1, i, 1) ;

		 glTextureParameteri(IDs_layer[i], GL_TEXTURE_WRAP_S, setting.WrapX);
		 glTextureParameteri(IDs_layer[i], GL_TEXTURE_WRAP_T, setting.WrapY);
		 glTextureParameteri(IDs_layer[i], GL_TEXTURE_MIN_FILTER, setting.Min);
		 glTextureParameteri(IDs_layer[i], GL_TEXTURE_MAG_FILTER, setting.Max);
	}

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}