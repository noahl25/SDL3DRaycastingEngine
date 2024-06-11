#pragma once

#include <vector>
#include <glm/gtc/epsilon.hpp >

#include "Camera.h"
#include "Renderer.h"
#include "Ray.h"
#include "Color.h"
#include "stb_image.h"
#include "Texture.h"

class World
{
public:

	World();
	~World();

	void Render(Renderer& renderer);
	void OnKeyPress(SDL_Scancode scancode);
	void OnMousePress(int x, int y);
 

private:

	RaycastHit CastRay(const Ray& ray);

	void Render2D(Renderer& renderer);
	void Render3D(Renderer& renderer);
	
	bool MapCollision(const glm::vec2& location);

private:

	std::vector<int> m_Map = {
		1, 1, 1, 1, 1,
		1, 0, 0, 0, 1,
		1, 0, 1, 0, 1,
		1, 0, 0, 0, 1,
		1, 1, 1, 1, 1 
	};

	Camera m_Camera;

	Texture m_Texture;
	bool m_CollisionActive = false;
};

