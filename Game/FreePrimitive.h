#pragma once
#include <Dx12MSLib.h>

class FreePrimitive : public PrimitiveObject
{
public:
	FreePrimitive(const std::vector<PrimitiveVertex>& vertices, const std::vector<unsigned int>& indices);
	~FreePrimitive();
private:
};

