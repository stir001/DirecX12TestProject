#include "PMXLoader.h"
#include "PMXModel.h"

void LoadTexBuffer(TextBuffer& buff, FILE* fp, size_t size = -1)
{
	unsigned int count = 0;
	fread(&count, sizeof(unsigned int), 1, fp);
	if (count == 0) return;
	buff.buffer.resize(count);
	fread(&buff.buffer[0], count * sizeof(unsigned char), 1, fp);
}

PMXLoader::PMXLoader()
{
}


PMXLoader::~PMXLoader()
{
}

template <typename T>
void LoadOwnSize(T& t, FILE* fp)
{
	fread(&t, sizeof(t), 1, fp);
}

bool CheckBit(unsigned short bit, unsigned short checkbit)
{
	return ((bit & checkbit) == checkbit);
}

//pmxを過程
void PMXLoader::Load(std::string path) {

	FILE* pmxfp = nullptr;
	fopen_s(&pmxfp, path.data(), "rb");

	PMXModel* model = new PMXModel();

	//ヘッダ
	LoadOwnSize(model->header.type, pmxfp);
	LoadOwnSize(model->header.ver, pmxfp);
	LoadOwnSize(model->header.count, pmxfp);

	model->header.info.resize(model->header.count);
	fread(&model->header.info[0], sizeof(model->header.info[0]), model->header.info.size(), pmxfp);

	//モデル情報(名前やコメントなど)
	LoadTexBuffer(model->modelinfo.nameJP, pmxfp);
	LoadTexBuffer(model->modelinfo.nameUS, pmxfp);
	LoadTexBuffer(model->modelinfo.commentJP, pmxfp);
	LoadTexBuffer(model->modelinfo.commentUS, pmxfp);

	models.push_back(model);

	//頂点
	LoadVertex(pmxfp);

	//面
	LoadPlane(pmxfp);

	//テクスチャ
	LoadTexture(pmxfp);

	//マテリアル
	LoadMaterial(pmxfp);

	//ボーン
	LoadBone(pmxfp);

	//モーフ
	LoadMorph(pmxfp);

	//表示枠
	LoadOutLine(pmxfp);

	//剛体
	LoadRigid(pmxfp);

	//joint
	LoadJoint(pmxfp);

	fclose(pmxfp);


}

void PMXLoader::LoadVertex(FILE* pmxfp)
{
	unsigned char boneIndexSize = models.back()->header.info[BONEINDEXSIZE];
	unsigned int vertexcount = 0;
	LoadOwnSize(vertexcount, pmxfp);
	models.back()->vertex.resize(vertexcount);

	for (unsigned int i = 0; i < vertexcount; i++)
	{
		models.back()->vertex[i].addVertex.resize(16 * models.back()->header.info[ADDUV]);
		fread(&models.back()->vertex[i].pos, 32, 1, pmxfp);
		fread(&models.back()->vertex[i].addVertex, 16 * 16 * models.back()->header.info[ADDUV], 1, pmxfp);
		fread(&models.back()->vertex[i].type, 1, 1, pmxfp);
		switch (models.back()->vertex[i].type)
		{
		case 0:
			fread(&models.back()->vertex[i].bone.data.bdef1.index, boneIndexSize, 1, pmxfp);
			break;
		case 1:
			fread(&models.back()->vertex[i].bone.data.bdef2.index1, boneIndexSize, 1, pmxfp);
			fread(&models.back()->vertex[i].bone.data.bdef2.index2, boneIndexSize, 1, pmxfp);
			fread(&models.back()->vertex[i].bone.data.bdef2.weight1, sizeof(float), 1, pmxfp);
			break;
		case 2:
			fread(&models.back()->vertex[i].bone.data.bdef4.index1, boneIndexSize, 1, pmxfp);
			fread(&models.back()->vertex[i].bone.data.bdef4.index2, boneIndexSize, 1, pmxfp);
			fread(&models.back()->vertex[i].bone.data.bdef4.index3, boneIndexSize, 1, pmxfp);
			fread(&models.back()->vertex[i].bone.data.bdef4.index4, boneIndexSize, 1, pmxfp);
			fread(&models.back()->vertex[i].bone.data.bdef4.weight1, sizeof(float), 1, pmxfp);
			fread(&models.back()->vertex[i].bone.data.bdef4.weight2, sizeof(float), 1, pmxfp);
			fread(&models.back()->vertex[i].bone.data.bdef4.weight3, sizeof(float), 1, pmxfp);
			fread(&models.back()->vertex[i].bone.data.bdef4.weight4, sizeof(float), 1, pmxfp);
			break;
		case 3:
			fread(&models.back()->vertex[i].bone.data.sdef.index1, boneIndexSize, 1, pmxfp);
			fread(&models.back()->vertex[i].bone.data.sdef.index2, boneIndexSize, 1, pmxfp);
			fread(&models.back()->vertex[i].bone.data.sdef.weight1, sizeof(float), 1, pmxfp);
			fread(&models.back()->vertex[i].bone.data.sdef.c, 12, 1, pmxfp);
			fread(&models.back()->vertex[i].bone.data.sdef.r0, 12, 1, pmxfp);
			fread(&models.back()->vertex[i].bone.data.sdef.r1, 12, 1, pmxfp);
			break;
		default:
			break;
		}
		fread(&models.back()->vertex[i].edgscale, sizeof(float), 1, pmxfp);
	}
}

void PMXLoader::LoadPlane(FILE* pmxfp)
{
	unsigned int indexcount = 0;
	LoadOwnSize(indexcount, pmxfp);
	indexcount /= 3;
	models.back()->planes.index.resize(indexcount);
	for (int i = 0; i < indexcount; ++i)
	{
		fread(&models.back()->planes.index[i].x, models.back()->header.info[VERTEXINDEXSIZE], 1, pmxfp);
		fread(&models.back()->planes.index[i].y, models.back()->header.info[VERTEXINDEXSIZE], 1, pmxfp);
		fread(&models.back()->planes.index[i].z, models.back()->header.info[VERTEXINDEXSIZE], 1, pmxfp);
	}
}

void PMXLoader::LoadTexture(FILE* pmxfp)
{
	unsigned int texturecount = 0;
	LoadOwnSize(texturecount, pmxfp);
	models.back()->texPath.resize(texturecount);
	for (int i = 0; i < texturecount; i++)
	{
		LoadTexBuffer(models.back()->texPath[i], pmxfp);
	}
}

void PMXLoader::LoadMaterial(FILE* pmxfp)
{
	unsigned int materialcount = 0;
	fread(&materialcount, sizeof(unsigned int), 1, pmxfp);
	models.back()->materials.resize(materialcount);
	for (auto& m : models.back()->materials)
	{
		LoadTexBuffer(m.nameJP, pmxfp);

		LoadTexBuffer(m.nameUS, pmxfp);

		LoadOwnSize(m.diffuse, pmxfp);
		LoadOwnSize(m.specular, pmxfp);
		LoadOwnSize(m.specularrato, pmxfp);
		LoadOwnSize(m.ambient, pmxfp);

		LoadOwnSize(m.drawFlag, pmxfp);

		LoadOwnSize(m.edgecolor, pmxfp);
		LoadOwnSize(m.edgesize, pmxfp);

		fread(&m.tableindex, models.back()->header.info[TEXINDEXSIZE], 1, pmxfp);
		fread(&m.spheretableindex, models.back()->header.info[TEXINDEXSIZE], 1, pmxfp);
		LoadOwnSize(m.spheremode, pmxfp);

		LoadOwnSize(m.toonFlag, pmxfp);

		if (m.toonFlag == 0)
		{
			fread(&m.toonindex, sizeof(models.back()->header.info[TEXINDEXSIZE]), 1, pmxfp);
		}
		else if (m.toonFlag == 1)
		{
			fread(&m.toonindex, sizeof(unsigned char), 1, pmxfp);
		}
		else
		{
			int i = 0;//でバック用
		}

		LoadTexBuffer(m.freetext, pmxfp);
		LoadOwnSize(m.adaptVertexcount, pmxfp);
	}
}

void PMXLoader::LoadBone(FILE* pmxfp)
{
	unsigned int bonecount = 0;
	fread(&bonecount, sizeof(unsigned int), 1, pmxfp);
	models.back()->bonedata.resize(bonecount);
	for (auto& bone : models.back()->bonedata)
	{
		LoadTexBuffer(bone.boneNameJP, pmxfp);
		LoadTexBuffer(bone.boneNameUS, pmxfp);

		LoadOwnSize(bone.pos, pmxfp);
		fread(&bone.parentIndex, models.back()->header.info[BONEINDEXSIZE], 1, pmxfp);
		LoadOwnSize(bone.formlevel, pmxfp);
		LoadOwnSize(bone.boneflag, pmxfp);

		if (CheckBit(bone.boneflag, 0x0001))
		{
			fread(&bone.conectboneIndex, models.back()->header.info[BONEINDEXSIZE], 1, pmxfp);
		}
		else
		{
			LoadOwnSize(bone.offsetpos, pmxfp);
		}

		if (CheckBit(bone.boneflag, 0x0100))
		{
			fread(&bone.addparentboneIndex, models.back()->header.info[BONEINDEXSIZE], 1, pmxfp);
			LoadOwnSize(bone.addparentRato, pmxfp);
		}

		if (CheckBit(bone.boneflag, 0x0200))
		{
			fread(&bone.addparentboneIndex, models.back()->header.info[BONEINDEXSIZE], 1, pmxfp);
			LoadOwnSize(bone.addparentRato, pmxfp);
		}

		if (CheckBit(bone.boneflag, 0x0400))
		{
			LoadOwnSize(bone.dirvec, pmxfp);
		}

		if (CheckBit(bone.boneflag, 0x0800))
		{
			LoadOwnSize(bone.xAxisvec, pmxfp);
			LoadOwnSize(bone.zAxisvec, pmxfp);
		}

		if (CheckBit(bone.boneflag, 0x2000))
		{
			LoadOwnSize(bone.key, pmxfp);
		}

		if (CheckBit(bone.boneflag, 0x0020))
		{
			fread(&bone.ikdata.targetboneIndex, sizeof(models.back()->header.info[BONEINDEXSIZE]), 1, pmxfp);
			fseek(pmxfp, 1, SEEK_CUR);
			LoadOwnSize(bone.ikdata.roopcount, pmxfp);
			LoadOwnSize(bone.ikdata.maxrad, pmxfp);
			LoadOwnSize(bone.ikdata.linkcount, pmxfp);
			bone.ikdata.links.resize(bone.ikdata.linkcount);
			for (auto& ik : bone.ikdata.links)
			{
				fread(&ik.linkboneIndex, models.back()->header.info[BONEINDEXSIZE], 1, pmxfp);
				LoadOwnSize(ik.maxradflag, pmxfp);
				if (ik.maxradflag == 1)
				{
					LoadOwnSize(ik.minrad, pmxfp);
					LoadOwnSize(ik.maxrad, pmxfp);
				}
			}
		}
	}
}

void PMXLoader::LoadMorph(FILE* pmxfp)
{
	unsigned int morphcount;
	LoadOwnSize(morphcount, pmxfp);
	models.back()->morphDatas.resize(morphcount);
	for (auto& data : models.back()->morphDatas)
	{
		LoadTexBuffer(data.nameJP, pmxfp);
		LoadTexBuffer(data.nameUS, pmxfp);
		LoadOwnSize(data.controllpanel, pmxfp);
		LoadOwnSize(data.morphtype, pmxfp);
		LoadOwnSize(data.morphcount, pmxfp);
		data.morphs.resize(data.morphcount);
		for (auto& morph : data.morphs)
		{
			switch (data.morphtype)
			{
			case mt_vertex:
				fread(&morph.vMorph.vertexIndex, models.back()->header.info[VERTEXINDEXSIZE], 1, pmxfp);
				LoadOwnSize(morph.vMorph.offsetpos.x, pmxfp);
				LoadOwnSize(morph.vMorph.offsetpos.y, pmxfp);
				LoadOwnSize(morph.vMorph.offsetpos.z, pmxfp);
				break;
			case mt_uv:
				fread(&morph.uMorph.vertexIndex, models.back()->header.info[VERTEXINDEXSIZE], 1, pmxfp);
				LoadOwnSize(morph.uMorph.uvoffset.x, pmxfp);
				LoadOwnSize(morph.uMorph.uvoffset.y, pmxfp);
				break;
			case mt_uvadd1:
				fread(&morph.uMorph.vertexIndex, models.back()->header.info[VERTEXINDEXSIZE], 1, pmxfp);
				LoadOwnSize(morph.uMorph.uvoffset.x, pmxfp);
				LoadOwnSize(morph.uMorph.uvoffset.y, pmxfp);
				LoadOwnSize(morph.uMorph.uvoffset.z, pmxfp);
				break;
			case mt_uvadd2:
				fread(&morph.uMorph.vertexIndex, models.back()->header.info[VERTEXINDEXSIZE], 1, pmxfp);
				LoadOwnSize(morph.uMorph.uvoffset.x, pmxfp);
				LoadOwnSize(morph.uMorph.uvoffset.y, pmxfp);
				LoadOwnSize(morph.uMorph.uvoffset.z, pmxfp);
				LoadOwnSize(morph.uMorph.uvoffset.w, pmxfp);
				break;
			case mt_bone:
				fread(&morph.bMorph.boneIndex, models.back()->header.info[BONEINDEXSIZE], 1, pmxfp);
				LoadOwnSize(morph.bMorph.velocity, pmxfp);
				LoadOwnSize(morph.bMorph.rotator, pmxfp);
				break;
			case mt_material:
				fread(&morph.mMorph.materialIndex, models.back()->header.info[MATERIALINDEXSIZE], 1, pmxfp);
				LoadOwnSize(morph.mMorph.offsetcalType, pmxfp);
				LoadOwnSize(morph.mMorph.diffuse, pmxfp);
				LoadOwnSize(morph.mMorph.specular, pmxfp);
				LoadOwnSize(morph.mMorph.specularrato, pmxfp);
				LoadOwnSize(morph.mMorph.ambient, pmxfp);
				LoadOwnSize(morph.mMorph.edgecolor, pmxfp);
				LoadOwnSize(morph.mMorph.edgesize, pmxfp);
				LoadOwnSize(morph.mMorph.texrato, pmxfp);
				LoadOwnSize(morph.mMorph.spheretexrato, pmxfp);
				LoadOwnSize(morph.mMorph.toontexrato, pmxfp);
				break;
			case mt_group:
				fread(&morph.gMorph.index, models.back()->header.info[MORPHINDEXSIZE], 1, pmxfp);
				LoadOwnSize(morph.gMorph.mophrato, pmxfp);
				break;
			default:
				break;
			}
		}
	}
}

void PMXLoader::LoadOutLine(FILE* pmxfp)
{
	unsigned int outlincount = 0;
	LoadOwnSize(outlincount, pmxfp);
	models.back()->outline.resize(outlincount);

	for (auto& o : models.back()->outline)
	{
		LoadTexBuffer(o.nameJP, pmxfp);
		LoadTexBuffer(o.nameUS, pmxfp);

		LoadOwnSize(o.flag, pmxfp);
		LoadOwnSize(o.count, pmxfp);
		o.datas.resize(o.count);
		for (auto& d : o.datas)
		{
			LoadOwnSize(d.type, pmxfp);
			switch (d.type)
			{
			case 0://ボーン
				fread(&d.index, models.back()->header.info[BONEINDEXSIZE], 1, pmxfp);
				break;
			case 1://モーフ
				fread(&d.index, models.back()->header.info[MORPHINDEXSIZE], 1, pmxfp);
				break;
			default:
				break;
			}
		}
	}
}

void PMXLoader::LoadRigid(FILE* pmxfp)
{
	unsigned int rigidcount = 0;
	LoadOwnSize(rigidcount, pmxfp);
	models.back()->rigid.resize(rigidcount);

	for (auto& r : models.back()->rigid)
	{
		LoadTexBuffer(r.nameJP, pmxfp);
		LoadTexBuffer(r.nameUS, pmxfp);

		fread(&r.boneIndex, models.back()->header.info[BONEINDEXSIZE], 1, pmxfp);

		LoadOwnSize(r.group, pmxfp);
		LoadOwnSize(r.groupflag, pmxfp);

		LoadOwnSize(r.shape, pmxfp);
		LoadOwnSize(r.size, pmxfp);

		LoadOwnSize(r.pos, pmxfp);
		LoadOwnSize(r.rotator, pmxfp);

		LoadOwnSize(r.weight, pmxfp);
		LoadOwnSize(r.accelvel, pmxfp);
		LoadOwnSize(r.accelrota, pmxfp);
		LoadOwnSize(r.boundpw, pmxfp);
		LoadOwnSize(r.frictionpw, pmxfp);

		LoadOwnSize(r.rigidflag, pmxfp);
	}
}

void PMXLoader::LoadJoint(FILE* pmxfp)
{
	unsigned int jointcount = 0;
	LoadOwnSize(jointcount, pmxfp);
	models.back()->joints.resize(jointcount);
	for (auto& j : models.back()->joints)
	{
		LoadTexBuffer(j.nameJP, pmxfp);
		LoadTexBuffer(j.nameUS, pmxfp);
		LoadOwnSize(j.type, pmxfp);
		fread(&j.spring.rigidAIndex, models.back()->header.info[RIGIDINDEXSIZE], 1, pmxfp);
		fread(&j.spring.rigidBIndex, models.back()->header.info[RIGIDINDEXSIZE], 1, pmxfp);
		LoadOwnSize(j.spring.pos, pmxfp);
		LoadOwnSize(j.spring.rotator, pmxfp);
		LoadOwnSize(j.spring.minVel, pmxfp);
		LoadOwnSize(j.spring.maxVel, pmxfp);
		LoadOwnSize(j.spring.minRotator, pmxfp);
		LoadOwnSize(j.spring.maxRotator, pmxfp);
		LoadOwnSize(j.spring.springCVel, pmxfp);
		LoadOwnSize(j.spring.springCRota, pmxfp);

	}
}