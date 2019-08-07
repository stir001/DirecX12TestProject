#include "GameDataLoader.h"
#include <Dx12MSLib.h>

const std::string DATA_FILE_PATH= "Game/Data/";
const std::string CHECKER = "CUTCUTCUT";

GameDataLoader::GameDataLoader()
{
}


GameDataLoader::~GameDataLoader()
{
}

GameDataLoader::GameData GameDataLoader::LoadGameData(const std::string& dataPath)
{
	GameData rtn;

	File file(dataPath);

	unsigned int headerStrNum = static_cast<unsigned int>(CHECKER.size());
	std::string header;
	header.resize(headerStrNum);
	file.LoadFile(&header[0], headerStrNum);
	if (CHECKER != header)
	{
		file.Close();
		return;
	}

	unsigned int dataNameNum = 0U;
	file.LoadFile(&dataNameNum);
	std::string dataName;
	dataName.resize(dataNameNum);
	file.LoadFile(&dataName[0], dataNameNum);

	unsigned int vertsNum = 0U;
	file.LoadFile(&vertsNum);
	rtn.verts.resize(vertsNum);
	file.LoadFile(&rtn.verts[0], vertsNum);

	unsigned int indicesNum = 0U;
	file.LoadFile(&indicesNum);
	rtn.indices.resize(indicesNum);
	file.LoadFile(&rtn.indices[0], indicesNum);

	return rtn;
}

void GameDataLoader::WriteGameData(const std::string& fileName, const GameData& data)
{
	File file;
	file.SetWriteFile(DATA_FILE_PATH + fileName);

	file.Write(CHECKER.data(), static_cast<int>(CHECKER.size()));

	unsigned int questStrLen = static_cast<unsigned int>(fileName.size());
	file.Write(&questStrLen);
	file.Write(fileName.data(), questStrLen);

	unsigned int vertNum = static_cast<unsigned int>(data.verts.size());
	file.Write(&vertNum);
	file.Write(data.verts.data(), vertNum);

	unsigned int indicesNum = static_cast<unsigned int>(data.indices.size());
	file.Write(&indicesNum);
	file.Write(data.indices.data(), indicesNum);

	file.Close();
}
