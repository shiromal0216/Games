#include <DxLib.h>
#include "Block.h"


#define BLOCK_SIZE			(60)

Block::Block():
	blockState(0),
	blockSingleColor(-1),
	blockMixColor(-1),
	positionX(0.0f),
	positionY(0.0f)
{

}

Block::~Block()
{

}

void Block::Initialize(float x, float y)
{
	blockState = 0;
	blockSingleColor = -1;
	blockMixColor = -1;

	positionX = x;
	positionY = y;

	boxCollider.Initialize(positionX, positionY, positionX + BLOCK_SIZE, positionY + BLOCK_SIZE);
}

void Block::Update()
{

}

void Block::Render()
{
	if (blockState == 1) 
	{
		DrawBox(positionX, positionY, positionX + BLOCK_SIZE, positionY + BLOCK_SIZE,
			GetColor(blockSingleColorR[blockSingleColor], blockSingleColorG[blockSingleColor], blockSingleColorB[blockSingleColor]), 1);
	}
	else if (blockState == 2)
	{
		DrawBox(positionX, positionY, positionX + BLOCK_SIZE, positionY + BLOCK_SIZE,
			GetColor(blockMixColorR[blockMixColor], blockMixColorG[blockMixColor], blockMixColorB[blockMixColor]), 1);
	}
	else if (blockState == 4)
	{
		DrawBox(positionX, positionY, positionX + BLOCK_SIZE, positionY + BLOCK_SIZE,
			GetColor(blockMixColorR[3], blockMixColorG[3], blockMixColorB[3]), 1);
	}

}

void Block::Finalize()
{

}

void Block::SetPosition(float x, float y)
{
	positionX = x;
	positionY = y;
}

void Block::SetState(int state)
{
	blockState = state;
}

void Block::SetSingleColor(int color)
{
	blockSingleColor = color;
}

void Block::SetMixColor(int color)
{
	blockMixColor = color;
}

bool Block::CheckHitBlock(float mouseX, float mouseY)
{
	if (boxCollider.CheckHit(mouseX, mouseY, mouseX + 1.0f, mouseY + 1.0f) != boxCollider.AABB_NONE)
	{
		return true;
	}

	return false;

}