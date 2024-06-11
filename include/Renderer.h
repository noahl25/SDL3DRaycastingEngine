#pragma once

#include <SDL.h>
#include <cassert>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <functional>

#include "Engine.h"

class Renderer
{
public:

	Renderer(int width, int height);
	~Renderer();

	inline void Init(SDL_Renderer* renderer) { 
		assert(renderer != nullptr);
		m_Renderer = renderer;
		m_Texture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, m_Width, m_Height);
		SDL_SetRenderTarget(m_Renderer, m_Texture);
		m_Texture = SDL_GetRenderTarget(m_Renderer);
		SDL_SetTextureBlendMode(m_Texture, SDL_BLENDMODE_BLEND);
	}

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline SDL_Renderer* GetRenderer() const { return m_Renderer; }

	inline void Present() {
		SDL_SetRenderTarget(m_Renderer, NULL);
		SDL_RenderCopy(m_Renderer, m_Texture, NULL, NULL);
		SDL_RenderPresent(m_Renderer);
		SDL_SetRenderTarget(m_Renderer, m_Texture);
	}

	inline void Clear(const glm::vec4& color = glm::vec4(0.0f)) {
		SDL_SetRenderDrawColor(m_Renderer, EXPAND_UINT32(Engine::AsUInt32(color)));
		SDL_SetRenderTarget(m_Renderer, NULL);
		SDL_RenderClear(m_Renderer);
		SDL_SetRenderTarget(m_Renderer, m_Texture);
		SDL_RenderClear(m_Renderer);
	}

	inline void Rect(const SDL_Rect& rect, const glm::vec4& color = glm::vec4(1.0f)) const {
		SDL_SetRenderDrawColor(m_Renderer, EXPAND_UINT32(Engine::AsUInt32(color)));
		SDL_RenderFillRect(m_Renderer, &rect);
	}

	inline void Line(const glm::vec2& first, const glm::vec2& second, const glm::vec4& color = glm::vec4(1.0f)) const {
		SDL_SetRenderDrawColor(m_Renderer, EXPAND_UINT32(Engine::AsUInt32(color)));
		SDL_RenderDrawLine(m_Renderer, first.x, first.y, second.x, second.y);
	}

	inline void Blit(SDL_Texture* texture) {
		SDL_RenderCopy(m_Renderer, texture, NULL, NULL);
	}

private:

	SDL_Renderer* m_Renderer;
	SDL_Texture* m_Texture;

	int m_Width, m_Height;

	bool m_Locked = false;

};

