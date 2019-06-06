#pragma once
#include <random>

class RandomGenerator
{
public:
	RandomGenerator(float min, float max);
	~RandomGenerator();

	float Get() const;
	float Get(float min, float max);
private:
	std::uniform_real_distribution<float> mRandom;
};

