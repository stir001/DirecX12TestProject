#pragma once
#include "ModelLoader.h"

#include <string>
#include <vector>

class PMXModel;
struct PMXVertex;
struct PMXPlane;
struct TextBuffer;
struct PMXMatarial;
struct PMXBoneData;
struct PMXMorph;
struct PMXOutLine;
struct PMXRigidBody;
struct PMXJoint;

class PMXLoader :
	public ModelLoader
{
private:
	std::vector<PMXModel*> models;

	void LoadVertex(FILE* pmxfp);
	void LoadPlane(FILE* pmxfp);
	void LoadTexture(FILE* pmxfp);
	void LoadMaterial(FILE* pmxfp);
	void LoadBone(FILE* pmxfp);
	void LoadMorph(FILE* pmxfp);
	void LoadOutLine(FILE* pmxfp);
	void LoadRigid(FILE* pmxfp);
	void LoadJoint(FILE* pmxfp);
public:
	void Load(std::string path);
	PMXLoader();
	~PMXLoader();
};

