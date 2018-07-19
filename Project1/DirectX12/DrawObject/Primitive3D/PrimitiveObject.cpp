#include "PrimitiveObject.h"
#include "VertexBufferObject.h"
#include <d3d12.h>


PrimitiveObject::PrimitiveObject()
{
}


PrimitiveObject::~PrimitiveObject()
{
}

void PrimitiveObject::SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelineState)
{
	mPipelineState = pipelineState;
}

void PrimitiveObject::SetRootsignature(std::shared_ptr<RootSignatureObject>& rootsiganature)
{
	mRootsignature = rootsiganature;
}
