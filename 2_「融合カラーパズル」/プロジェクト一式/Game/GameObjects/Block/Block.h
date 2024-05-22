#pragma once
#include "../../GameDatas/Collision.h"

class Block
{
private:
	int blockState;	//ブロックの状態（０：未表示、１：単色表示、２：混合色表示、３：空ブロック、４：お邪魔ブロック）

	int blockSingleColor;	//ブロックの色（０：赤、１：黄色、２：青）
	int blockMixColor;		//ブロックの色（０：オレンジ、１：緑、２：紫、３：灰色）
	
	float positionX;
	float positionY;

	int blockSingleColorR[3] = { 255, 255,   0 };
	int blockSingleColorG[3] = {   0, 255,   0 };
	int blockSingleColorB[3] = {   0,   0, 255 };

	int blockMixColorR[4] = { 255,  0,128,128 };
	int blockMixColorG[4] = { 165,128,  0,128 };
	int blockMixColorB[4] = { 0,    0,128,128 };

	BoxCollider boxCollider;

public:
	Block();
	~Block();

	void Initialize(float x, float y);
	void Update();
	void Render();
	void Finalize();

	void SetPosition(float x, float y);
	float GetPositionX() { return positionX; };
	float GetPositionY() { return positionY; };

	void SetState(int state);
	int GetState() { return blockState; };

	void SetSingleColor(int color);
	int GetBlockSingleColor() { return blockSingleColor; };

	void SetMixColor(int color);
	int GetBlockMixColor() { return blockMixColor; };

	bool CheckHitBlock(float mouseX, float mouseY);
};