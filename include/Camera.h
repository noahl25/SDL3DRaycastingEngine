#pragma once

#include <glm/glm.hpp>

struct Camera {
	int FOV = 60;

	glm::vec2 Position = glm::vec2(0.0f, 0.0f);
	float Yaw = 25.0f;
};