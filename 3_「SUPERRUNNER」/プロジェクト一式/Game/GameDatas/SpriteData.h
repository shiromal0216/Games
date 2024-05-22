#pragma once
#include "Utility.h"

class SpriteData
{
private:
	// 座標
	Position pos;

	// 切り取りの終了位置
	Pixel textureSize;

	// 切り取りの開始位置
	Pixel src;

	// 切り取りの終了位置
	Pixel size;

	// 画像の中心位置
	Position center;

	// 拡大率
	Vector scale;

	// 角度（radian）
	double rot;

	// 透明度
	float alpha;

	// 画像ハンドル
	int textureHandle;

	unsigned int mFlag;

public:

private:
public:
	SpriteData();
	~SpriteData();

	// 画像ハンドルの設定
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

	// 描画
	void Render();
};


