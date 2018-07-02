#pragma once
#include "ICharactor.h"

class PlayerSH;

class Enemy :
	public ICharactor
{
protected:
	std::weak_ptr<PlayerSH> mwpPlayer;
public:
	Enemy(std::shared_ptr<ImageController>& imgCtrl,std::shared_ptr<PlayerSH> mspPlayer);
	virtual ~Enemy();
};

