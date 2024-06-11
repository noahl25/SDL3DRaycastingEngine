#pragma once

#include <SDL.h>
#include <cassert>
#include <cstring>
#include <glm/glm.hpp>

#include "Timer.h"
#include "World.h"
#include "Renderer.h"

class Game
{
public:

	Game(int width, int height);
	~Game();

	void Run();

private:

	void HandleEvents();
	void Update();

private:

	SDL_Window* m_Window;
	Renderer m_Renderer;

	SDL_Event m_Event{};

	int m_Width, m_Height;
	bool m_Running = true;
	bool m_Paused = false;

	glm::vec2 m_PlayerPos = glm::vec2(2.0f, 2.0f);
	float m_PlayerYaw = 0.0f;

	World world;
};

