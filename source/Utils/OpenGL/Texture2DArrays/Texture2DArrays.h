#ifndef TEXTURE_2D_ARRAYS_H
#define TEXTURE_2D_ARRAYS_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <string>
#include <stdexcept>
#include <vector>
#include <iostream>  
#include <cassert>

#include "Utils/File/File.h"
#include "Utils/OpenGL/Enums/OpenGL.Enums.h"
#include "Utils/OpenGL/TextureStructs/TextureStructs.h"

class Texture2DArrays {
public:
	Texture2DArrays();
	Texture2DArrays(const std::vector<std::string>& paths_to_images);
	~Texture2DArrays();

	void LoadImages(const std::vector<std::string>& paths_to_images);
	void Init();

	void Bind();
	void SetActive(const TextureActiveNumber& number);
	const GLuint& GetID();
	const GLuint& GetID_Layer(const int& layer_index);

	const int& getWidth();
	const int& getHeight();
	const int& getCountLayers();

private:
	GLuint ID;
	std::vector<GLuint> IDs_layer;
	int width, height, countLayers;
	std::vector<uint8_t*> images_data;
	Texture2DSetting setting;
};


#endif