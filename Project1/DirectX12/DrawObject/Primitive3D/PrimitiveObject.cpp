#include "PrimitiveObject.h"
#include "VertexBufferObject.h"
#include <d3d12.h>


PrimitiveObject::PrimitiveObject(const std::string& name): mName(name)
{
}


PrimitiveObject::~PrimitiveObject()
{
}

const std::string & PrimitiveObject::GetName() const
{
	return mName;
}

std::vector<PrimitiveVertex>& PrimitiveObject::GetVertices()
{
	return mVertices;
}

