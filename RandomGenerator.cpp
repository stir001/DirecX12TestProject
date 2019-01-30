#include "RandomGenerator.h"



RandomGenerator::RandomGenerator() : mSeedGen() ,mEngine(mSeedGen())
{
}


RandomGenerator::~RandomGenerator()
{
}

float RandomGenerator::GetValue(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(mEngine);
}
