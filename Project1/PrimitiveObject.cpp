#include "PrimitiveObject.h"
#include <d3d12.h>


PrimitiveObject::PrimitiveObject()
{
}


PrimitiveObject::~PrimitiveObject()
{
	vertexBuffer->Release();
	delete vbView;
	delete mapData;
}
