#ifndef TEXTURE_STRUCTS_H
#define TEXTURE_STRUCTS_H
#include "Utils/OpenGL/Enums/OpenGL.Enums.h"

struct Texture2DSetting {
	TextureWrap WrapX = TextureWrap::CLAMP_TO_EDGE;
	TextureWrap WrapY = TextureWrap::CLAMP_TO_EDGE;
	TextureFilter Min = TextureFilter::LINEAR;
	TextureFilter Max = TextureFilter::LINEAR;
};

struct TextureDataSetting {
	int width;
	int height;
	int MipMapLevel;
	TextureInternalPixelFormat internalFormat;
	TexturePixelFormat format;
	TextureDataType type;
};

struct TextureInfo {
	int width = 0;
	int height = 0;
	int channels = 0;
};

#endif