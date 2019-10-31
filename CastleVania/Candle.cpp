#include "Candle.h"

void Candle::SetHit()
{
	isHitted = true;
}

void Candle::Render()
{
	if (!isHitted)
		animations[0]->Render(x, y, 255);
}

void Candle::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + CANDLE_BBOX_WIDTH;
	b = y + CANDLE_BBOX_HEIGHT;
}
