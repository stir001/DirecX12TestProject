#pragma once
#include <random>

class RandomGenerator
{
public:
	RandomGenerator();
	~RandomGenerator();

	float GetValue(float min, float max);

private:
	std::random_device mSeedGen;
	std::mt19937 mEngine;
};

