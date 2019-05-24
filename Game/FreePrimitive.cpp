#include "FreePrimitive.h"
#include "PrimitiveCutter.h"
#include <random>

FreePrimitive::FreePrimitive(const std::vector<PrimitiveVertex>& vertices
	, const std::vector<unsigned int>& indices) : PrimitiveObject("FreePrimitive")
{
	mVertices = vertices;
	mIndices = indices;
}

FreePrimitive::~FreePrimitive()
{
}

