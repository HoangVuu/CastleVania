#include "GameObject.h"
#pragma once

#define BRICK_BBOX_WIDTH  1600
#define BRICK_BBOX_HEIGHT 16

class Ground : public CGameObject
{	
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};
