#include "Engine.h"

namespace Engine {

	uint32_t AsUInt32(const glm::vec4& color)
	{
		return (((uint8_t)(color.r * 255.0f)) << 24) | (((uint8_t)(color.g * 255.0f)) << 16) | (((uint8_t)(color.b * 255.0f)) << 8) | (uint8_t)(color.a * 255.0f);
	}

    uint32_t AsUInt32(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        uint32_t color = 0;
        color |= (r << 24);
        color |= (g << 16);
        color |= (b << 8);
        color |= a;
        return color;
    }

	glm::vec2 AsDirectionV(float yaw)
	{
		return glm::normalize(glm::vec2(glm::cos(glm::radians(yaw)), glm::sin(glm::radians(yaw))));
	}
    void Engine::BHLine(uint32_t* buffer, int width, int height, const glm::uvec2& first, const glm::uvec2& second, const glm::vec4& color)
    {
        if (first == second) return;
        if (first.x < 0 || first.y < 0 || second.x < 0 || second.y < 0) return;

        uint32_t uColor = AsUInt32(color);

        if (first.x == second.x) {

            glm::vec2 less;
            glm::vec2 greater;

            if (first.y < second.y) {
                less = first;
                greater = second;
            }
            else {
                less = second;
                greater = first;
            }

            for (unsigned i = less.y; i <= greater.y; i += 1) {
                if (InBounds(i * width + first.x, width, height))
                    buffer[i * width + first.x] = uColor;
            }

            return;

        }

        if (first.y == second.y) {

            glm::vec2 less;
            glm::vec2 greater;

            if (first.x < second.x) {
                less = first;
                greater = second;
            }
            else {
                less = second;
                greater = first;
            }

            for (unsigned i = less.x; i <= greater.x; i += 1)
                if (InBounds(first.y * width + i, width, height))
                    buffer[first.y * width + i] = uColor;

            return;

        }

        int x1 = first.x;
        int x2 = second.x;
        int y1 = height - first.y;
        int y2 = height - second.y;

        int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;

        dx = x2 - x1;
        dy = y2 - y1;

        dx1 = std::abs(dx);
        dy1 = std::abs(dy);

        px = 2 * dy1 - dx1;
        py = 2 * dx1 - dy1;

        if (dy1 <= dx1) {

            if (dx >= 0) {
                x = x1; y = y1; xe = x2;
            }
            else {
                x = x2; y = y2; xe = x1;
            }
            if (InBounds((height - y) * width + x, width, height))
                buffer[(height - y) * width + x] = uColor;
            for (i = 0; x < xe; i++) {
                x = x + 1;

                if (px < 0) {
                    px = px + 2 * dy1;
                }
                else {
                    if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                        y = y + 1;
                    }
                    else {
                        y = y - 1;
                    }
                    px = px + 2 * (dy1 - dx1);
                }

                if (InBounds((height - y) * width + x, width, height))
                    buffer[(height - y) * width + x] = uColor;
            }
        }
        else {
            if (dy >= 0) {
                x = x1; y = y1; ye = y2;
            }
            else {
                x = x2; y = y2; ye = y1;
            }
            if (InBounds((height - y) * width + x, width, height))
                buffer[(height - y) * width + x] = uColor;
            for (i = 0; y < ye; i++) {
                y = y + 1;

                if (py <= 0) {
                    py = py + 2 * dx1;
                }
                else {
                    if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                        x = x + 1;
                    }
                    else {
                        x = x - 1;
                    }
                    py = py + 2 * (dx1 - dy1);
                }
                if (InBounds((height - y) * width + x, width, height))
                    buffer[(height - y) * width + x] = uColor;
            }
        }

    }
    bool InBounds(int val, int width, int height)
    {
        if (val < 0)
            return false;
        if (val >= width * height)
            return false;
        return true;
    }

};