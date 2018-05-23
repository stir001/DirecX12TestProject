#pragma once
#include "ICharactor.h"
class Enemy :
	public ICharactor
{
public:
	Enemy(std::shared_ptr<ImageController>& imgCtrl);
	virtual ~Enemy();
};

