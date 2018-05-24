#pragma once
#include <memory>

class ICharactor;

class ColliisonDetector
{
public:
	ColliisonDetector();
	~ColliisonDetector();


	void CheckCollision(std::shared_ptr<ICharactor>& c1, std::shared_ptr<ICharactor>& c2);
private:
};

