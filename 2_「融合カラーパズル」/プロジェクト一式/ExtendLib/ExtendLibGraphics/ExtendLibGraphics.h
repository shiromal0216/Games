/*
	ExtendLibGraphics.h
	DxLib���g�p���āA�����֗��ɂ����@�\����������N���X

	�쐬�ҁ@�F�@�x��@�a��
	�쐬���@�F�@2021/03/16
*/

#pragma once


class ExtendLibGraphics
{
private:
	static const int NUM_CAPSULE_DIVISION;

public:
	ExtendLibGraphics();
	~ExtendLibGraphics();

	// �j����`��
	int DrawDashedLine(int x1, int y1, int x2, int y2, unsigned int color, int length, int thickness = 1);
	// �j����`��(�A���`�G�C���A�X���ʕt��)
	int DrawDashedLineAA(float x1, float y1, float x2, float y2, unsigned int color, float length, float thickness = 1.0f);
	// �J�v�Z����`��
	int DrawCapsule(int x1, int y1, int x2, int y2, int r, unsigned int color, int thickness = 1);
	// �J�v�Z����`��(�A���`�G�C���A�X���ʕt��)
	int DrawCapsuleAA(float x1, float y1, float x2, float y2, float r, unsigned int color, float thickness = 1.0f);
	// �����x�t���̎��i��`��
	void DrawAlphaBox(float x1, float y1, float x2, float y2, unsigned int color, float alpha);


};