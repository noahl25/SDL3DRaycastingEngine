#pragma once

#include <iostream>

#ifndef DEBUG
#define DEBUG
#endif

#ifdef DEBUG

#define ASSERT(condition) if (!condition) __debugbreak();
#define LOG_VEC4(vec4) std::cout << vec4.x << ", " << vec4.y << ", " << vec4.z << ", " << vec4.w << "\n";

#else

#define ASSERT
#define LOG_VEC4

#endif 

