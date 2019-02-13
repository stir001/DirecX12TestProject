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

	ActionData& LoadActionData(std::string path);
private:
	File* mFile;
	std::map<std::string, ActionData> mActions;

	void LoadActionHeader(ActionHeader& header);
	std::vector<Action> LoadActionRects();
};

