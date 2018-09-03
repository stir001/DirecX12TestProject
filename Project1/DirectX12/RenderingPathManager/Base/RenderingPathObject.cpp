#include "RenderingPathObject.h"



RenderingPathObject::RenderingPathObject()
{
}


RenderingPathObject::~RenderingPathObject()
{
}

void RenderingPathObject::SetActive(bool isActive)
{
	mIsActive = isActive;
}

bool RenderingPathObject::IsActive() const
{
	return mIsActive;
}
