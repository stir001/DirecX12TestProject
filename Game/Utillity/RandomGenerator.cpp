#include "RandomGenerator.h"


RandomGenerator::RandomGenerator(float min, float max)
{
	mRandom = std::uniform_real_distribution<float>(min, max);
}

RandomGenerator::~RandomGenerator()
{
}

float RandomGenerator::Get() const
{
	std::random_device dev;
	std::mt19937 mt(dev());
	return mRandom(mt);
}

float RandomGenerator::Get(float min, float max)
{
	mRandom = std::uniform_real_distribution<float>(min, max);
	return Get();
}
