#pragma once

#include<Windows.h>

struct XINPUT_STATE;

struct MousePos
{
	POINT pos;
};

//window側で定義されていない部分の
//仮想キーコードの定義と
//ゲームで使うことの多い仮想キーコードのenum化
enum VirturalKeyIndex
{
	//未定義の部分を定義
	vki_0 = 0x30,
	vki_1 = 0x31,
	vki_2 = 0x32,
	vki_3 = 0x33,
	vki_4 = 0x34,
	vki_5 = 0x35,
	vki_6 = 0x36,
	vki_7 = 0x37,
	vki_8 = 0x38,
	vki_9 = 0x39,
	vki_A = 0x41,
	vki_B = 0x42,
	vki_C = 0x43,
	vki_D = 0x44,
	vki_E = 0x45,
	vki_F = 0x46,
	vki_G = 0x47,
	vki_H = 0x48,
	vki_I = 0x49,
	vki_J = 0x4A,
	vki_K = 0x4B,
	vki_L = 0x4C,
	vki_M = 0x4D,
	vki_N = 0x4E,
	vki_O = 0x4F,
	vki_P = 0x50,
	vki_Q = 0x51,
	vki_R = 0x52,
	vki_S = 0x53,
	vki_T = 0x54,
	vki_U = 0x55,
	vki_V = 0x56,
	vki_W = 0x57,
	vki_X = 0x58,
	vki_Y = 0x59,
	vki_Z = 0x5A,

	//使うことの多い部分のenum化
	vki_LBUTTON = 0x01,//マウス左ボタン
	vki_RBUTTON = 0x02,//マウス右ボタン
	vki_MBUTTON = 0x04,//マウス中央ボタン
	vki_ENTER	= 0x0D,//Enter(Return)キー
	vki_SHIFT	= 0x10,//Shiftキー(左右を区別しない)
	vki_CONTROL = 0x11,//Ctrlキー(左右を区別しない)
	//vki_ALT		= 0x12,//Altキー(左右区別しない)	なぜか常に押している状態が返ってくる
	vki_LSHIFT	= 0xA0,//左Shiftキー
	vki_RSHIFT	= 0xA1,//右Shiftキー
	vki_LCONTROL= 0xA2,//左Ctrlキー
	vki_RCONTROL= 0xA3,//右Ctrlキー
	vki_LALT	= 0xA4,//左Altキー
	vki_RALT	= 0xA5,//右Altキー
	vki_ESCAPE	= 0x1B,//ESCキー
	vki_SPACE	= 0x20,//Spaceキー
	vki_LEFT	= 0x25,//左矢印キー
	vki_UP		= 0x26,//上矢印キー
	vki_RIGHT	= 0x27,//右矢印キー
	vki_DOWN	= 0x28,//下矢印キー
	vki_NUMPAD0 = 0x60,//ナムパッド0キー
	vki_NUMPAD1 = 0x61,//ナムパッド1キー
	vki_NUMPAD2 = 0x62,//ナムパッド2キー
	vki_NUMPAD3 = 0x63,//ナムパッド3キー
	vki_NUMPAD4 = 0x64,//ナムパッド4キー
	vki_NUMPAD5 = 0x65,//ナムパッド5キー
	vki_NUMPAD6 = 0x66,//ナムパッド6キー
	vki_NUMPAD7 = 0x67,//ナムパッド7キー
	vki_NUMPAD8 = 0x68,//ナムパッド8キー
	vki_NUMPAD9 = 0x69,//ナムパッド9キー
};

enum KeyStateCheck
{
	ksc_DOWN	= 0x80,//キーが押されているかどうかの判定
	ksc_TOGGLE	= 0x01,//トグルがonかoffかの判定
};

class DxInput
{
private:
	//最上位ビット(0x80)が立っているときは今押されている
	//再下位ビット(0x01)はトグルのon off判定
	//どちらもたっていないときは押されていない
	//以下のサイトを参照
	//ttp://www016.upp.so-net.ne.jp/garger-studio/gameprog/vb0142.html
	unsigned char keyState[256];
	MousePos mousePos;
	//XINPUT_STATE* xInState;
public:
	bool UpdateKeyState();
	const MousePos GetMousePos();

	//@return 256の要素をもつ配列を返す
	const unsigned char* GetKeyState();

	bool CheckKeyDown(VirturalKeyIndex index);
	DxInput();
	~DxInput();
};

