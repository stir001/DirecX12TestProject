#pragma once
/**
*	@addtogroup PMD_Data
*	@file PMDDataStructure
*	@brief PMDの詳細な情報を保持する構造体の定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/20
*/
#include <vector>
#include <DirectXMath.h>

/**
*	@ingroup PMD_Data
*	@struct PMDHeader
*	@brief PMDファイルのヘッダ部分の構造体
*/
struct PMDHeader
{
	/**
	*	PMDという文字が入っているヘッダ部分
	*/
	char fileType[3];

	/**
	*	PMDのバージョン情報
	*/
	float version;

	/**
	*	PMDモデルの名前
	*/
	char name[20];

	/**
	*	PMDモデルについてのコメント
	*/
	char comment[256];

	/**
	*	頂点数
	*/
	unsigned int vertexcount;
};

/**
*	@ingroup PMD_Data
*	@struct PMDVertex
*	@brief PMDの頂点情報
*/
struct PMDVertex
{
	/**
	*	頂点座標
	*/
	DirectX::XMFLOAT3 pos;

	/**
	*	法線情報
	*/
	DirectX::XMFLOAT3 normal;

	/**
	*	uv情報
	*/
	DirectX::XMFLOAT2 uv;

	/**
	*	影響を受けるボーン番号
	*/
	unsigned short boneNum[2];

	/**
	*	ボーン番号の0番目のボーンから影響を受けるウェイト
	*/
	unsigned char boneWeight;//ウェイト(1バイト) 
	unsigned char edgeFlag;//輪郭線フラグ(1バイト) 
};

/**
*	@ingroup PMD_Data
*	@struct クラス名
*	@brief クラスの詳細
*/
struct PMDMaterial {
	DirectX::XMFLOAT3 diffuse;
	float alpha;
	float specularity;
	DirectX::XMFLOAT3 specular;
	DirectX::XMFLOAT3 ambient;
	unsigned char toonIndex;
	unsigned char edgeFlag;
	unsigned int indexCount;
	char texturePath[20];
	int texid;
};

/**
*	@ingroup PMD_Data
*	@struct クラス名
*	@brief クラスの詳細
*/
struct PMDBoneData
{
	char boneName[20];//ボーン名
	unsigned short parentIndex;//親番号
	unsigned short tailIndex;//終端番号
	unsigned char type;//種類
	unsigned short ikParentIndex;//IK親番号
	DirectX::XMFLOAT3 pos;//座標
};

/**
*	@ingroup PMD_Data
*	@struct クラス名
*	@brief クラスの詳細
*/
struct PMDIKData
{
	unsigned short ikBoneIndex;
	unsigned short ikTargetBoneIndex;
	unsigned char ikChainLength;//IKチェーンの長さ
	unsigned short iterations;//再起演算回数 IK値1
	float controlWeight;//演算一回当たりの制限角度 IK値2
	std::vector<unsigned short> ikChildBoneIndex;//IK影響かのボーン番号(要素数はikChainLength数)
};

/**
*	@ingroup PMD_Data
*	@struct クラス名
*	@brief クラスの詳細
*/
struct PMDIKList
{
	unsigned short ikCount;
	std::vector<PMDIKData> ikDatas;
};

/**
*	@ingroup PMD_Data
*	@struct クラス名
*	@brief クラスの詳細
*/
struct PMDSkinVertexData
{
	unsigned int index;
	DirectX::XMFLOAT3 pos;
};

/**
*	@ingroup PMD_Data
*	@struct クラス名
*	@brief クラスの詳細
*/
struct PMDSkinData
{
	char skinName[20];
	unsigned int skinvertexcount;
	char skintype;//0:base,1:まゆ,2:目,3:リップ,4:その他
	std::vector<PMDSkinVertexData> vertexeis;
};

/**
*	@ingroup PMD_Data
*	@struct クラス名
*	@brief クラスの詳細
*/
struct PMDSkin
{
	unsigned short skincount;
	std::vector<PMDSkinData> datas;
};

/**
*	@ingroup PMD_Data
*	@struct クラス名
*	@brief クラスの詳細
*/
struct PMDSkinDisp
{
	unsigned char skindispcount;
	std::vector<unsigned short> skinIndices;
};

/**
*	@ingroup PMD_Data
*	@struct クラス名
*	@brief クラスの詳細
*/
struct PMDBoneFrameDispName
{
	char name[50];
};

/**
*	@ingroup PMD_Data
*	@struct クラス名
*	@brief クラスの詳細
*/
struct PMDBoneFrameDisp
{
	unsigned char boneDispNameCount;
	std::vector<PMDBoneFrameDispName> names;
};

/**
*	@ingroup PMD_Data
*	@struct クラス名
*	@brief クラスの詳細
*/
struct PMDBoneDispIndex
{
	unsigned short boneIndex;
	unsigned char boneDispFrameIndex;
};

/**
*	@ingroup PMD_Data
*	@struct
*	@brief
*/
struct PMDBoneDisp
{
	unsigned int boneocunt;//0以外のボーン数の合計
	std::vector<PMDBoneDispIndex> boneDispIndices;
};

/**
*	@ingroup PMD_Data
*	@struct
*	@brief
*/
struct PMDToonPath
{
	char path[100][10];
};

/**
*	@ingroup PMD_Data
*	@struct
*	@brief
*/
struct PMDRigidBodyData
{
	char rigidBodyName[20];
	unsigned short rigidbodyRelBoneIndex;//関連ボーン番号
	unsigned char rigidbodyGroupIndex;
	unsigned short rigidbodyGroupTarget;
	unsigned char shapeType; // 形状：タイプ(0:球、1:箱、2:カプセル) // 00 // 球
	float shapeW; // 形状：半径(幅) // CD CC CC 3F // 1.6
	float shapeH; // 形状：高さ // CD CC CC 3D // 0.1
	float shapeD; // 形状：奥行 // CD CC CC 3D // 0.1
	DirectX::XMFLOAT3 pos; // 位置：位置(x, y, z)
	DirectX::XMFLOAT3 rot; // 位置：回転(rad(x), rad(y), rad(z))ラジアン
	float weight; // 諸データ：質量 // 00 00 80 3F // 1.0
	float posDim; // 諸データ：移動減 // 00 00 00 00
	float rotDim; // 諸データ：回転減 // 00 00 00 00
	float recoil; // 諸データ：反発力 // 00 00 00 00
	float friction; // 諸データ：摩擦力 // 00 00 00 00
	unsigned char rigidbodyType; // 諸データ：タイプ(0:Bone追従、1:物理演算、2:物理演算(Bone位置合せ)) // 00 // Bone追従
};

/**
*	@ingroup PMD_Data
*	@struct
*	@brief
*/
struct PMDRigdBody
{
	unsigned int rigidbodycount;
	std::vector<PMDRigidBodyData> datas;
};

/**
*	@ingroup PMD_Data
*	@struct
*	@brief
*/
struct PMDJointData
{
	char jointName[20]; // 諸データ：名称 // 右髪1
	unsigned int jointRigidbodyA; // 諸データ：剛体A
	unsigned int jointRigidbodyB; // 諸データ：剛体B
	DirectX::XMFLOAT3 jointPos; // 諸データ：位置(x, y, z) // 諸データ：位置合せでも設定可
	DirectX::XMFLOAT3 jointRot; // 諸データ：回転(rad(x), rad(y), rad(z))
	DirectX::XMFLOAT3 constrainPos1; // 制限：移動1(x, y, z)
	DirectX::XMFLOAT3 constrainPos2; // 制限：移動2(x, y, z)
	DirectX::XMFLOAT3 constrainRot1; // 制限：回転1(rad(x), rad(y), rad(z))
	DirectX::XMFLOAT3 constrainRot2; // 制限：回転2(rad(x), rad(y), rad(z))
	DirectX::XMFLOAT3 springPos; // ばね：移動(x, y, z)
	DirectX::XMFLOAT3 springRot; // ばね：回転(rad(x), rad(y), rad(z))
};

/**
*	@ingroup PMD_Data
*	@struct
*	@brief
*/
struct PMDJoint
{
	unsigned int jointcount;
	std::vector<PMDJointData> datas;
};

/**
*	@ingroup PMD_Data
*	@struct
*	@brief
*/
struct BoneTree
{
	std::vector<std::vector<int>> node;
};