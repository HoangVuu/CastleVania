#pragma once
#include "GameObject.h"
#include "Whip.h"
#define	SIMON_LEVEL_SMALL	1
#define	SIMON_LEVEL_BIG		2




class Simon : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;
	bool isJump;
	bool isSit;
	bool isRight;
	bool isAttack;
	bool isCollideHorizontal;

	DWORD attackTime;

public:

	Whip *whip;

	Simon() : CGameObject()
	{
		level = SIMON_LEVEL_BIG;
		untouchable = 0;
		whip = new Whip();

		isJump = false;
		isSit = false;
		isRight = true;
		isAttack = false;
	}

	//xét va chạm của Simon
	virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	//void setWhip(Whip *a) { whip = a; }
	void SetPosition(float x, float y) {
		this->x = x;
		this->y = y;
		whip->SetPosition(x+10, y+10);
	}
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};
