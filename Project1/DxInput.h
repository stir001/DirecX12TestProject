#pragma once

#include<Windows.h>

struct XINPUT_STATE;

struct MousePos
{
	POINT pos;
};

//window���Œ�`����Ă��Ȃ�������
//���z�L�[�R�[�h�̒�`��
//�Q�[���Ŏg�����Ƃ̑������z�L�[�R�[�h��enum��
enum VirturalKeyIndex
{
	//����`�̕������`
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

	//�g�����Ƃ̑���������enum��
	vki_LBUTTON = 0x01,//�}�E�X���{�^��
	vki_RBUTTON = 0x02,//�}�E�X�E�{�^��
	vki_MBUTTON = 0x04,//�}�E�X�����{�^��
	vki_ENTER	= 0x0D,//Enter(Return)�L�[
	vki_SHIFT	= 0x10,//Shift�L�[(���E����ʂ��Ȃ�)
	vki_CONTROL = 0x11,//Ctrl�L�[(���E����ʂ��Ȃ�)
	//vki_ALT		= 0x12,//Alt�L�[(���E��ʂ��Ȃ�)	�Ȃ�����ɉ����Ă����Ԃ��Ԃ��Ă���
	vki_LSHIFT	= 0xA0,//��Shift�L�[
	vki_RSHIFT	= 0xA1,//�EShift�L�[
	vki_LCONTROL= 0xA2,//��Ctrl�L�[
	vki_RCONTROL= 0xA3,//�ECtrl�L�[
	vki_LALT	= 0xA4,//��Alt�L�[
	vki_RALT	= 0xA5,//�EAlt�L�[
	vki_ESCAPE	= 0x1B,//ESC�L�[
	vki_SPACE	= 0x20,//Space�L�[
	vki_LEFT	= 0x25,//�����L�[
	vki_UP		= 0x26,//����L�[
	vki_RIGHT	= 0x27,//�E���L�[
	vki_DOWN	= 0x28,//�����L�[
	vki_NUMPAD0 = 0x60,//�i���p�b�h0�L�[
	vki_NUMPAD1 = 0x61,//�i���p�b�h1�L�[
	vki_NUMPAD2 = 0x62,//�i���p�b�h2�L�[
	vki_NUMPAD3 = 0x63,//�i���p�b�h3�L�[
	vki_NUMPAD4 = 0x64,//�i���p�b�h4�L�[
	vki_NUMPAD5 = 0x65,//�i���p�b�h5�L�[
	vki_NUMPAD6 = 0x66,//�i���p�b�h6�L�[
	vki_NUMPAD7 = 0x67,//�i���p�b�h7�L�[
	vki_NUMPAD8 = 0x68,//�i���p�b�h8�L�[
	vki_NUMPAD9 = 0x69,//�i���p�b�h9�L�[
};

enum KeyStateCheck
{
	ksc_DOWN	= 0x80,//�L�[��������Ă��邩�ǂ����̔���
	ksc_TOGGLE	= 0x01,//�g�O����on��off���̔���
};

class DxInput
{
private:
	//�ŏ�ʃr�b�g(0x80)�������Ă���Ƃ��͍�������Ă���
	//�ĉ��ʃr�b�g(0x01)�̓g�O����on off����
	//�ǂ���������Ă��Ȃ��Ƃ��͉�����Ă��Ȃ�
	//�ȉ��̃T�C�g���Q��
	//ttp://www016.upp.so-net.ne.jp/garger-studio/gameprog/vb0142.html
	unsigned char keyState[256];
	MousePos mousePos;
	//XINPUT_STATE* xInState;
public:
	bool UpdateKeyState();
	const MousePos GetMousePos();

	//@return 256�̗v�f�����z���Ԃ�
	const unsigned char* GetKeyState();

	bool CheckKeyDown(VirturalKeyIndex index);
	DxInput();
	~DxInput();
};

