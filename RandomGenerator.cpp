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

unsigned int RandomGenerator::GetValue(unsigned int min, unsigned int max)
{
	//std::uniform_real_distribution<unsigned int> dist(min, max);
	return 0;
}

int RandomGenerator::GetValue(int min, int max)
{
	//std::uniform_real_distribution<int> dist(min, max);
	return 0;
}
