#include "ShaderDatasStructure.h"

ShaderDatas::ShaderDatas()
{

}

ShaderDatas::~ShaderDatas()
{
	rootSignature.Reset();
	vertexShader.Reset();
	pixcelShader.Reset();
	geometryShader.Reset();
	hullShader.Reset();
}