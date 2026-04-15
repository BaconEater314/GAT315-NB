#pragma once

#include "raylib.h"

class Random
{
public:
	inline static float GetRandomFloat()
	{
		return GetRandomValue(0, 10000) / (float)10000;
	}

	inline static float GetRandomFloat(float max)
	{
		return GetRandomValue(0, max) / (float)10000;
	}

	inline static float GetRandomFloat(float min, float max)
	{
		return GetRandomValue(min, max) / (float)10000;
	}
};