#include "Game.h"

Game::Game(int width, int height)
	: m_Width(width), m_Height(height), m_Renderer(1000, 500)
{

	assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);
	m_Window = SDL_CreateWindow("SDL Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Width, m_Height, NULL);
	m_Renderer.Init(SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED));
	 

}

void Game::Run()
{

	const int FPSTarget = 60;
	const int deltaTimeTarget = 1000 / FPSTarget;

	unsigned frameStart;
	unsigned frameTime;

	unsigned prevTime = SDL_GetTicks();

	while (m_Running) {

		frameStart = SDL_GetTicks();
		HandleEvents();
		if (!m_Paused) {

			Update();


			frameTime = SDL_GetTicks() - frameStart;

			if (deltaTimeTarget > frameTime)
				SDL_Delay(deltaTimeTarget - frameTime);

		}

	}


}

void Game::HandleEvents()
{
	while ((SDL_PollEvent(&m_Event)) != 0)
	{
		if (m_Event.type == SDL_QUIT)
		{
			m_Running = false;
		}

		if (m_Event.type == SDL_MOUSEBUTTONDOWN) {

			int x, y;
			SDL_GetMouseState(&x, &y);
			world.OnMousePress(x, y);

		}

		if (m_Event.type == SDL_KEYDOWN) {
			if (m_Event.key.keysym.sym == SDLK_p)
				m_Paused = !m_Paused;
		}
	}

	const Uint8* keyState = SDL_GetKeyboardState(NULL);

	if (keyState[SDL_SCANCODE_D]) {
		world.OnKeyPress(SDL_SCANCODE_D);
	}
	if (keyState[SDL_SCANCODE_A]) {
		world.OnKeyPress(SDL_SCANCODE_A);
	}
	if (keyState[SDL_SCANCODE_W]) {
		world.OnKeyPress(SDL_SCANCODE_W);
	}
	if (keyState[SDL_SCANCODE_S]) {
		world.OnKeyPress(SDL_SCANCODE_S);
	}

	if (keyState[SDL_SCANCODE_E]) {
		world.OnKeyPress(SDL_SCANCODE_E);
	}
	if (keyState[SDL_SCANCODE_Q]) {
		world.OnKeyPress(SDL_SCANCODE_Q);
	}

}


void Game::Update()
{

	m_Renderer.Clear();


	world.Render(m_Renderer);

	m_Renderer.Present();

}


Game::~Game()
{
	SDL_DestroyWindow(m_Window);
}