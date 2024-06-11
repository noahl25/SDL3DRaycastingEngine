#pragma once

#include <glm/glm.hpp>
#include "stb_image.h"

class Texture
{
public:

	Texture(const char* path);
	~Texture();

	glm::vec4 PixelAt(int x, int y);

private:

	unsigned char* m_Data;
	int m_Width, m_Height;
};

