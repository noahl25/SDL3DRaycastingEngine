#include "Texture.h"

Texture::Texture(const char* path)
{
	int channels;

	unsigned char* data = stbi_load(path, &m_Width, &m_Height, &channels, 4);
	if (data == NULL) __debugbreak();

	m_Data = data;
}


glm::vec4 Texture::PixelAt(int x, int y)
{
	glm::vec4 pixel = glm::vec4(0.0f);
	pixel.r = (int)m_Data[(y * m_Width + x) * 4] / 255.0f;
	pixel.g = (int)m_Data[(y * m_Width + x) * 4 + 1] / 255.0f;
	pixel.b = (int)m_Data[(y * m_Width + x) * 4 + 2] / 255.0f;
	pixel.a = (int)m_Data[(y * m_Width + x) * 4 + 3] / 255.0f;
	return pixel;
}

Texture::~Texture()
{
	stbi_image_free(m_Data);
}