#include "Renderer.h"

Renderer::Renderer(int width, int height)
	: m_Width(width), m_Height(height)
{
	 
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(m_Renderer);
}
