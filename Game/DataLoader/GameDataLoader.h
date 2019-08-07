#pragma once
#include <vector>
#include <string>

class PrimitiveVertex;

class GameDataLoader
{
public:
	GameDataLoader();
	~GameDataLoader();

	struct GameData
	{
		std::vector<PrimitiveVertex> verts;
		std::vector<unsigned int> indices;
	};

	GameData LoadGameData(const std::string& dataPath);

	void WriteGameData(const std::string& fileName, const GameData& data);
};

