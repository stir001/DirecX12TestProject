#pragma once
#include "ActionStructs.h"
#include <map>
#include <string>

class File;

class ActionLoader
{
public:
	ActionLoader();
	~ActionLoader();

	std::vector<Action>& LoadActionData(std::string path);
private:
	File* mFile;
	std::map<std::string, std::vector<Action>> mActions;

	void LoadActionHeader();
	std::vector<Action> LoadActionRects();
};

