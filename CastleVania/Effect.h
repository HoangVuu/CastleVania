#pragma once
#include "GameObject.h"

#define EFFECT_TIME						450

class Effect : public CGameObject
{
	
public:
	bool isExposed;
	DWORD firstRenderTime;
	Effect(DWORD time) { firstRenderTime = time; isExposed = false; }
	bool GetExposed() { return isExposed; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

