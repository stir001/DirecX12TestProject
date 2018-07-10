#include "ShaderDatasStructure.h"

ShaderDatas::ShaderDatas()
{

}

ShaderDatas::~ShaderDatas()
{
	rootSignature.Reset();
	vertexShader.Reset();
	pixelShader.Reset();
	geometryShader.Reset();
	hullShader.Reset();
}