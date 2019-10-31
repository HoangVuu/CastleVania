#pragma once
#include "GameObject.h"

#define CANDLE_BBOX_WIDTH  16
#define CANDLE_BBOX_HEIGHT 16

class Candle : public CGameObject
{
public:
	bool isHitted;
	Candle() :CGameObject() {
		isHitted = false;
	}
	void SetHit();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

