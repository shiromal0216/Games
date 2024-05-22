#pragma once
#include "../../GameDatas/Collision.h"

class Block
{
private:
	int blockState;	//�u���b�N�̏�ԁi�O�F���\���A�P�F�P�F�\���A�Q�F�����F�\���A�R�F��u���b�N�A�S�F���ז��u���b�N�j

	int blockSingleColor;	//�u���b�N�̐F�i�O�F�ԁA�P�F���F�A�Q�F�j
	int blockMixColor;		//�u���b�N�̐F�i�O�F�I�����W�A�P�F�΁A�Q�F���A�R�F�D�F�j
	
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