#include "PrimitiveObject.h"
#include "VertexBufferObject.h"
#include <d3d12.h>


PrimitiveObject::PrimitiveObject()
{
}


PrimitiveObject::~PrimitiveObject()
{
	delete mVertexBuffer;
}
