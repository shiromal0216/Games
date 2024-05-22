//------------------------------------------//
//	制作者：松戸浩希						//
//	概　要：AABB判定のプロジェクト			//
//------------------------------------------//

#pragma once

#include "../../GameDatas/Utility.h"
#include "../../GameDatas/Collision.h"

class BoxObject
{
private:
	int boxID;

	//AABB当たり判定
	BoxCollider boxCollider;

	//足場の座標(左上の角)
	Position pos;

public:
	BoxObject();
	~BoxObject();

	//初期化
	void Initialize(float x1, float y1, float x2, float y2);

	void SetPosition(float x1, float y1);

	void SetScale(float width, float height);

	//	描画
	void Render();

	Position GetPosition();

	//当たり判定の取得
	BoxCollider GetBoxCollider();

	void Finalize();
};