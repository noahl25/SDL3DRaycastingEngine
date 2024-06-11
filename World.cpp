#include "World.h"
#include "Debug.h"

World::World()
	: m_Texture("cobble.png")
{
	m_Camera.Position = glm::vec2(2.5f, 1.9f);

}

void World::Render(Renderer& renderer)
{
	
	Render2D(renderer);
	Render3D(renderer);

}

void World::OnKeyPress(SDL_Scancode scancode)
{

	float moveSpeed = 0.05f;
	glm::vec2 nextMove = glm::vec2(0.0f, 0.0f);

	if (scancode == SDL_SCANCODE_W) 
		nextMove += glm::normalize(glm::vec2(glm::cos(glm::radians(m_Camera.Yaw)), glm::sin(glm::radians(m_Camera.Yaw)))) * moveSpeed;
	
	if (scancode == SDL_SCANCODE_S)
		nextMove -= glm::normalize(glm::vec2(glm::cos(glm::radians(m_Camera.Yaw)), glm::sin(glm::radians(m_Camera.Yaw)))) * moveSpeed;

	if (scancode == SDL_SCANCODE_D)
		nextMove += glm::normalize(glm::vec2(-glm::sin(glm::radians(m_Camera.Yaw)), glm::cos(glm::radians(m_Camera.Yaw)))) * moveSpeed;

	if (scancode == SDL_SCANCODE_A)
		nextMove -= glm::normalize(glm::vec2(-glm::sin(glm::radians(m_Camera.Yaw)), glm::cos(glm::radians(m_Camera.Yaw)))) * moveSpeed;

	if (m_CollisionActive) {

		glm::vec2 xStep = glm::vec2(m_Camera.Position.x + nextMove.x, m_Camera.Position.y);
		glm::vec2 yStep = glm::vec2(m_Camera.Position.x, m_Camera.Position.y + nextMove.y);

		if (!MapCollision(xStep)) {
			m_Camera.Position.x += nextMove.x;
		}
		if (!MapCollision(yStep)) {
			m_Camera.Position.y += nextMove.y;
		}
	}
	else {
		m_Camera.Position += nextMove;
	}

	if (scancode == SDL_SCANCODE_E)
		m_Camera.Yaw += 0.75f;
	if (scancode == SDL_SCANCODE_Q)
		m_Camera.Yaw -= 0.75f;

}

void World::OnMousePress(int x, int y)
{
	if (x < 0 || x > 500 || y < 0 || y > 500)
		return;

	m_Map[(y / 100) * 5 + (x / 100)] = (m_Map[(y / 100) * 5 + (x / 100)] == 0 ? 1 : 0);

}


void World::Render2D(Renderer& renderer)
{
	//map & grid
	int y = 0;
	for (int i = 0; i < 25; i++) {
		if (i != 0 && i % 5 == 0) {
			y += 100;
		}
		int val = m_Map[i];
		if (val == 1) {
			renderer.Rect({ (i % 5) * 100, y, 100, 100 });
		};
	}
	int x = 100;
	for (int i = 0; i < 4; i++) {
		renderer.Line({ x, 0 }, { x, 500 }, glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
		x += 100;
	}

	y = 100;
	for (int i = 0; i < 4; i++) {
		renderer.Line({ 0, y }, { 500, y }, glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
		y += 100;
	}

	//rays

	Ray ray;
	ray.Start = m_Camera.Position;

	glm::vec2 cameraDir = glm::normalize(glm::vec2(glm::cos(glm::radians(m_Camera.Yaw)), glm::sin(glm::radians(m_Camera.Yaw))));
	glm::vec2 cameraRight = glm::normalize(glm::vec2(-glm::sin(glm::radians(m_Camera.Yaw)), glm::cos(glm::radians(m_Camera.Yaw)))) * 1.3f;
 
	for (int i = -250; i < 250; i++) {
		
		ray.Direction = glm::normalize(cameraDir + cameraRight * ((float)i / 500.0f));
		RaycastHit hit = CastRay(ray);
		glm::vec2 raycastHitPos = hit.HitPos;
		if (hit.Hit)
			renderer.Line(m_Camera.Position * 100.0f + 5.0f, raycastHitPos * 100.0f, { 0.0f, 1.0f, 0.0f, 1.0f });
	}


	//player
	renderer.Rect({ (int)(m_Camera.Position.x * 100.0f), (int)(m_Camera.Position.y * 100.0f), 10, 10 }, { 1.0f, 0.0f, 0.0f, 1.0f });
	renderer.Line(m_Camera.Position * 100.0f + glm::vec2(5.0f, 5.0f), (glm::normalize(glm::vec2(glm::cos(glm::radians(m_Camera.Yaw)), glm::sin(glm::radians(m_Camera.Yaw)))) * 10.0f + m_Camera.Position * 100.0f) + glm::vec2(5.0f, 5.0f));
}

//possibly a better way to do this?
static float AngleBetween(const glm::vec2& first, const glm::vec2& second) {
	
	//normal equality causing issues
	if (glm::epsilonEqual(first.x, second.x, 0.001f) && glm::epsilonEqual(first.y, second.y, 0.001f))
		return 0;

	return glm::acos(glm::dot(first, second) / (glm::length(first) * glm::length(second)));
}

void World::Render3D(Renderer& renderer)
{

	Ray ray;
	ray.Start = m_Camera.Position;
 
	glm::vec2 cameraDir = glm::normalize(glm::vec2(glm::cos(glm::radians(m_Camera.Yaw)), glm::sin(glm::radians(m_Camera.Yaw))));
	glm::vec2 cameraRight = glm::normalize(glm::vec2(-glm::sin(glm::radians(m_Camera.Yaw)), glm::cos(glm::radians(m_Camera.Yaw))));

	SDL_Surface* surface = SDL_CreateRGBSurface(0, 1000, 500, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	uint32_t* pixels = static_cast<uint32_t*>(surface->pixels);

	for (int i = -250; i < 250; i++) {

	
		ray.Direction = glm::normalize(cameraDir + cameraRight * ((float)i / 500.0f));
		RaycastHit hit = CastRay(ray);


		if (hit.Hit) {

			float normalizedDist = hit.Distance * glm::cos(AngleBetween(cameraDir, ray.Direction));

			float lineHeight = (64 * renderer.GetHeight()) / (normalizedDist * 100);

			float step = 16.0f / (float)lineHeight;
			float texOffset = 0.0f;

			if (lineHeight > renderer.GetHeight()) {
				texOffset = (float)(lineHeight - renderer.GetHeight()) / 2.0f;
				lineHeight = renderer.GetHeight();
			}
			float lineOffset = (renderer.GetHeight() - lineHeight) / 2;

			//glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			//if (hit.Side == 0) color.r = 0.9f;
			//else color.r = 0.7f;

			//color /= glm::clamp(glm::log(normalizedDist), 1.0f, 3.0f);

			float texY = texOffset * step;

			float wallX = 0;
			if (hit.Side == 0) 
				wallX = hit.HitPos.y;
			else
				wallX = hit.HitPos.x;
			wallX -= glm::floor(wallX);

			int texX = (int)(wallX * 16.0f);

			if (hit.Side == 0 && ray.Direction.x > 0)
				texX = 15 - texX;
			if (hit.Side == 1 && ray.Direction.y < 0)
				texX = 15 - texX;
			
		
			for (int j = lineOffset; j <= lineOffset + lineHeight; j++) {

				glm::vec4 color = m_Texture.PixelAt(texX, (int)texY);
				if (hit.Side == 0) color *= 2.0f;
				color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
				color /= glm::clamp(glm::log(normalizedDist), 1.0f, 3.0f);

				pixels[j * 1000 + (i + 750)] = Engine::AsUInt32(color);

				texY += step;
				texY = glm::clamp(texY, 0.0f, 15.0f);

			}



			for (int j = lineOffset + lineHeight; j < renderer.GetHeight(); j++) {
				pixels[j * 1000 + (i + 750)] = Engine::AsUInt32(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
			}

			for (int j = 0; j < lineOffset; j++) {
				pixels[j * 1000 + (i + 750)] = Engine::AsUInt32(Color::Blue);
			}
			

			//renderer.Line({ i + 750, lineOffset }, { i + 750, lineOffset + lineHeight }, color);

			//std::cout << "Ray #" << i << ":\n";
			//std::cout << "-------------------------------\n";
			//std::cout << "Ray Hit? " << hit.Hit << "\n";
			//std::cout << "Ray Start: " << ray.Start.x << ", " << ray.Start.y << "\n";
			//std::cout << "Ray Direction: " << ray.Direction.x << ", " << ray.Direction.y << "\n";
			//std::cout << "Ray Hit: " << hit.HitPos.x << ", " << hit.HitPos.y << "\n";
			//std::cout << "Distance: " << hit.Distance << "\n\n";
			//std::cout << "Line Height: " << lineHeight << "\n";
			//std::cout << "Line Offset: " << lineOffset << "\n";
			//std::cout << "Color: " << color.r << ", " << color.g << ", " << color.b << ", " << color.a << "\n";
			//std::cout << "-------------------------------\n\n";

		}


	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer.GetRenderer(), surface);
	renderer.Blit(texture);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	
		 
}

bool World::MapCollision(const glm::vec2& location)
{
	if (m_Map[glm::floor(location.y) * 5 + glm::floor(location.x)] != 0)
		return true;
	return false;
}


RaycastHit World::CastRay(const Ray& ray)
{
	//algo from olc

	RaycastHit hit;

	glm::vec2 vRayStart = ray.Start;
	glm::vec2 vRayDir = ray.Direction;

	//glm::vec2 vRayUnitStepSize = { abs(1.0f / vRayDir.x), abs(1.0f / vRayDir.y) };

	glm::vec2 vRayUnitStepSize = { sqrt(1 + (vRayDir.y / vRayDir.x) * (vRayDir.y / vRayDir.x)), sqrt(1 + (vRayDir.x / vRayDir.y) * (vRayDir.x / vRayDir.y)) };
	glm::ivec2 vMapCheck = vRayStart;
	glm::vec2 vRayLength1D;
	glm::ivec2 vStep;

 
	if (vRayDir.x < 0)
	{
		vStep.x = -1;
		vRayLength1D.x = (vRayStart.x - float(vMapCheck.x)) * vRayUnitStepSize.x;
	}
	else
	{
		vStep.x = 1;
		vRayLength1D.x = (float(vMapCheck.x + 1) - vRayStart.x) * vRayUnitStepSize.x;
	}

	if (vRayDir.y < 0)
	{
		vStep.y = -1;
		vRayLength1D.y = (vRayStart.y - float(vMapCheck.y)) * vRayUnitStepSize.y;
	}
	else
	{
		vStep.y = 1;
		vRayLength1D.y = (float(vMapCheck.y + 1) - vRayStart.y) * vRayUnitStepSize.y;
	}
 
	bool bTileFound = false;
	float fMaxDistance = 100.0f;
	float fDistance = 0.0f;
	while (!bTileFound && fDistance < fMaxDistance)
	{
 
		if (vRayLength1D.x < vRayLength1D.y)
		{
			vMapCheck.x += vStep.x;
			fDistance = vRayLength1D.x;
			vRayLength1D.x += vRayUnitStepSize.x;
			hit.Side = 0;
		}
		else
		{
			vMapCheck.y += vStep.y;
			fDistance = vRayLength1D.y;
			vRayLength1D.y += vRayUnitStepSize.y;
			hit.Side = 1;
		}

 
		if (vMapCheck.x >= 0 && vMapCheck.x < 5 && vMapCheck.y >= 0 && vMapCheck.y < 5)
		{
			if (m_Map[vMapCheck.y * 5 + vMapCheck.x] == 1)
			{
				bTileFound = true;
			}
		}
	}


	if (!bTileFound) {
		return hit;
	}

	hit.HitPos = vRayStart + vRayDir * fDistance;
	hit.Distance = glm::distance(ray.Start, hit.HitPos);
	hit.Hit = true;

	if (hit.Side == 0) hit.PerpendicularDistance = (vMapCheck.x - vStep.x);
	else          hit.PerpendicularDistance = (vMapCheck.y - vStep.y);
 
	return hit;
	  
}


World::~World()
{
	 
}
