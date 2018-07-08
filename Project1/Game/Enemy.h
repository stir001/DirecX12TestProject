#pragma once
#include "ICharactor.h"

class PlayerSH;

class Enemy :
	public ICharactor
{
public:
	Enemy(std::shared_ptr<ImageController>& imgCtrl, std::shared_ptr<PlayerSH> mspPlayer);
	virtual ~Enemy();

protected:
	std::weak_ptr<PlayerSH> mwpPlayer;
};

