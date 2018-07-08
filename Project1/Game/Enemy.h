#pragma once
#include "ICharactor.h"

class PlayerSH;

class Enemy :
	public ICharactor
{
public:
	Enemy(std::shared_ptr<ImageController>& imgCtrl, std::shared_ptr<PlayerSH> mspPlayer);
	virtual ~Enemy();

	void SetID(int id);
	int GetID() const;
	bool IsDead() const;
protected:
	std::weak_ptr<PlayerSH> mwpPlayer;
	int mID;
	bool mIsDead;
};

