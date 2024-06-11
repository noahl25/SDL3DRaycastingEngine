#pragma once

#include <glm/glm.hpp>
#include <SDL.h>
#include <cstdint>

#define EXPAND_UINT32(uint32) \
    ((uint8_t)((uint32) >> 24)), ((uint8_t)((uint32) >> 16)), ((uint8_t)((uint32) >> 8)), ((uint8_t)((uint32) >> 0))

namespace Engine {

	uint32_t AsUInt32(const glm::vec4& color);
	uint32_t AsUInt32(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	glm::vec2 AsDirectionV(float yaw);
	void BHLine(uint32_t* buffer, int width, int height, const glm::uvec2& first, const glm::uvec2& second, const glm::vec4& color);
	bool InBounds(int val, int width, int height);

};
