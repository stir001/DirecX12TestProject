#include "IPrimitive2D.h"
#include "VertexBufferObject.h"


IPrimitive2D::IPrimitive2D(int vertexCount):mVertexBuffer(new VertexBufferObject(sizeof(Primitive2DVertex),vertexCount))
{
}


IPrimitive2D::~IPrimitive2D()
{
	delete mVertexBuffer;
}
