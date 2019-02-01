#pragma once
#include <random>

class RandomGenerator
{
public:
	RandomGenerator();
	~RandomGenerator();

	float GetValue(float min, float max);
	unsigned int GetValue(unsigned int min, unsigned int max);
	int GetValue(int min, int max);
private:
	std::random_device mSeedGen;
	std::mt19937 mEngine;
};

