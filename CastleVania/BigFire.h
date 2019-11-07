#pragma once
#include "GameObject.h"
#include "Effect.h"
#include"Item.h"

#define BIGFIRE_BBOX_WIDTH  32
#define BIGFIRE_BBOX_HEIGHT 32

class BigFire : public CGameObject
{
private:
	Item *item;
public:	
	Effect *effect;
	bool isHitted;
	bool isFirst;
	BigFire() :CGameObject() {	
		isHitted = false;
		isFirst = false;
		effect = new Effect(GetTickCount());
		effect->AddAnimation(500);
	}
	void SetHit();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};