#include "ActionLoader.h"
#include "File.h"


ActionLoader::ActionLoader()
{
}


ActionLoader::~ActionLoader()
{
}

ActionData& ActionLoader::LoadActionData(std::string path)
{
	auto itr = mActions.find(path);
	if (itr != mActions.end())
	{
		return itr->second;
	}
	mFile = new File(path);

	size_t szie = path.rfind('/');
	
	ActionData actData;

	LoadActionHeader(actData.header);
	actData.action = LoadActionRects();

	mActions[path] = actData;

	mFile->Close();
	delete mFile;

	return mActions[path];
}

void ActionLoader::LoadActionHeader(ActionHeader& header)
{
	mFile->LoadFile(&header.version);
	mFile->LoadFile(&header.fileNameSize);

	header.imagefilename.resize(header.fileNameSize);
	mFile->LoadFile(&header.imagefilename[0], 1, header.fileNameSize);
}

std::vector<Action> ActionLoader::LoadActionRects()
{
	std::vector<Action> actions;

	int actionCount;
	mFile->LoadFile(&actionCount);
	actions.resize(actionCount);

	DirectX::XMINT3 rCenter = {};
	DirectX::XMFLOAT3 rCenterf = {};
	DirectX::XMINT2 rSize;
	int actRectCount;
	int actionNameSize;
	int actionDataCount;

	for (auto& act : actions)
	{
		mFile->LoadFile(&actionNameSize);
		act.actionName.resize(actionNameSize);
		mFile->LoadFile(&act.actionName[0], actionNameSize);
		mFile->LoadFile(&act.isLoop);
		mFile->LoadFile(&actionDataCount);
		act.datas.resize(actionDataCount);
		for (auto& data : act.datas)
		{
			mFile->LoadFile(&rCenter.x);
			mFile->LoadFile(&rCenter.y);
			mFile->LoadFile(&rSize);

			rCenterf = { static_cast<float>(rCenter.x), static_cast<float>(rCenter.y), 0.0f };
			data.imageRect.SetCenter(rCenterf);
			data.imageRect.SetHeight(static_cast<float>(rSize.y));
			data.imageRect.SetWidth(static_cast<float>(rSize.x));

			mFile->LoadFile(&rSize);
			data.pivot = { static_cast<float>(rSize.x),static_cast<float>(rSize.y) };
			mFile->LoadFile(&data.duraction);
			mFile->LoadFile(&actRectCount);
			data.actionRects.resize(actRectCount);
			for (auto& actRect : data.actionRects)
			{
				mFile->LoadFile(&actRect.type);
				mFile->LoadFile(&rCenter.x);
				mFile->LoadFile(&rCenter.y);
				mFile->LoadFile(&rSize);

				rCenterf = { static_cast<float>(rCenter.x), static_cast<float>(rCenter.y), 0.0f };
				actRect.rect.SetCenter(rCenterf);
				actRect.rect.SetHeight(static_cast<float>(rSize.y));
				actRect.rect.SetWidth(static_cast<float>(rSize.x));
			}
		}
	}

	return actions;
}