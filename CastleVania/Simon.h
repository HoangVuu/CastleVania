#pragma once
#include "GameObject.h"
#define	SIMON_LEVEL_SMALL	1
#define	SIMON_LEVEL_BIG		2




class Simon : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;


public:

	Simon() : CGameObject()
	{
		level = SIMON_LEVEL_BIG;
		untouchable = 0;
	}


	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};