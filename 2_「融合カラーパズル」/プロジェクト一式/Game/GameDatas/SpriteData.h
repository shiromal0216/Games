#pragma once
#include "Utility.h"

class SpriteData
{
private:
	// ���W
	Position pos;

	// �؂���̏I���ʒu
	Pixel textureSize;

	// �؂���̊J�n�ʒu
	Pixel src;

	// �؂���̏I���ʒu
	Pixel size;

	// �摜�̒��S�ʒu
	Position center;

	// �g�嗦
	Vector scale;

	// �p�x�iradian�j
	double rot;

	// �����x
	float alpha;

	// �摜�n���h��
	int textureHandle;

	unsigned int mFlag;

public:

private:
public:
	SpriteData();
	~SpriteData();

	// �摜�n���h���̐ݒ�
	void SetRenderPosition(Position newPos);
	void SetTextureID(int handle);
	void SetCutData(Pixel cutPos, Pixel cutSize);
	void SetScale(float newScale);
	void SetRotate(float newRotate);
	void SetAlpha(float newAlpha);
	Position GetRenderPosition();
	Pixel GetCutPosition();
	Pixel GetCutSize();
	float GetScale();
	float GetRotate();
	float GetAlpha();

	void SetBrendFlag(unsigned int blendFlag);

	// �`��
	void Render();
};


