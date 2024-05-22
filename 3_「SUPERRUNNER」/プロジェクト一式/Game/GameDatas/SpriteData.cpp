#include <DxLib.h>
#include "SpriteData.h"


SpriteData::SpriteData():
	pos{ 0.0f, 0.0f },
	textureSize{ 0, 0 },
	src{ 0, 0 },
	size{ 0, 0 },
	center{ 0.5f, 0.5f },
	scale{ 1.0f, 1.0f },
	rot(0.0),
	alpha(1.0f),
	textureHandle(0),
	mFlag(DX_BLENDMODE_ALPHA)
{

}

SpriteData::~SpriteData()
{

}

void SpriteData::SetRenderPosition(Position newPos)
{
	pos = newPos;
}

//	âÊëúÉnÉìÉhÉãÇÃê›íË
void SpriteData::SetTextureID(int handle)
{
	textureHandle = handle;

	int width, height;

	GetGraphSize(textureHandle, &width, &height);

	textureSize.x = width;
	textureSize.y = height;
}

void SpriteData::SetCutData(Pixel cutPos, Pixel cutSize)
{
	src = cutPos;
	size = cutSize;

	center.x = size.x * scale.x / 2.0f;
	center.y = size.y * scale.y / 2.0f;

}

void SpriteData::SetScale(float newScale)
{
	scale.x = scale.y = newScale;

	center.x = size.x * scale.x / 2.0f;
	center.y = size.y * scale.y / 2.0f;
}

void SpriteData::SetRotate(float newRotate)
{
	rot = newRotate;
}

void SpriteData::SetAlpha(float newAlpha)
{
	alpha = newAlpha;
}

Position SpriteData::GetRenderPosition()
{
	return pos;
}

Pixel SpriteData::GetCutPosition()
{
	return src;
}

Pixel SpriteData::GetCutSize()
{
	return size;
}

float SpriteData::GetScale()
{
	return scale.x;
}

float SpriteData::GetRotate()
{
	return (float)rot;
}

float SpriteData::GetAlpha()
{
	return alpha;
}

void SpriteData::SetBrendFlag(unsigned int blendFlag)
{
	mFlag = blendFlag;
}

//	ï`âÊ
void SpriteData::Render()
{
	//	íÜâõà íuÇÃê›íË
	float cx = pos.x + center.x;
	float cy = pos.y + center.y;

	if (mFlag == DX_BLENDMODE_ADD ||
		mFlag == DX_BLENDMODE_SUB)
	{
		SetDrawBlendMode(mFlag, 128);
	}

	if (mFlag == DX_BLENDMODE_ALPHA)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(alpha * 255.0f));
	}

	DrawRectRotaGraph3F(
		pos.x, pos.y,
		src.x, src.y,
		size.x, size.y,
		center.x, center.y,
		(double)scale.x, (double)scale.y,
		rot,
		textureHandle,
		TRUE, TRUE
	);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
