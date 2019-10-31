#pragma once
#include "GameObject.h"

#define EFFECT_TIME						200

class Effect : public CGameObject
{
	bool isExposed;
	DWORD firstRenderTime;
public:
	Effect(DWORD time) { firstRenderTime = time; isExposed = false; }
	bool GetExposed() { return isExposed; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

