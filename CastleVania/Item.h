#pragma once
#include "GameObject.h"
#include "Ground.h"
#include "Candle.h"
#include "BigFire.h"


#define SIMON_GRAVITY					0.002f
#define ITEM_WHIPITEM					808
#define ITEM_HEART						805
#define ITEM_KNIFE						4433

// Big fire
#define BIGFIRE_BBOX_WIDTH				32
#define BIGFIRE_BBOX_HEIGHT				64

// Heart
#define HEART_BBOX_WIDTH				16
#define HEART_BBOX_HEIGHT				16

// Whip item
#define WHIPITEM_BBOX_WIDTH				32
#define WHIPITEM_BBOX_HEIGHT			32

// Knife
#define KNIFE_BBOX_WIDTH				32
#define KNIFE_BBOX_HEIGHT				18

#define KNIFE_SPEED						0.7f

class Item : public CGameObject
{

public:
	int type;
	bool isEaten;
	DWORD appearTime;

	void SetType(int _type) { type = _type; }
	int GetType() { return type; }
	void SetEaten() { isEaten = true; }
	bool GetEaten() { return isEaten; }

	virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

