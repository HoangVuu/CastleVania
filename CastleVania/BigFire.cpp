#include "BigFire.h"

void BigFire::Render()
{
	if (!isHitted)
		animations[0]->Render(x, y, 255);
}

void BigFire::SetHit()
{
	isHitted = true;
}

void BigFire::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BIGFIRE_BBOX_WIDTH;
	b = y + BIGFIRE_BBOX_HEIGHT;
}
