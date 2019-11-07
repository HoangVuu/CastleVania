#include "BigFire.h"

void BigFire::Render()
{
	if (!isHitted)
		animations[0]->Render(x, y, 255);
	else if (isHitted && !effect->isExposed)
		effect->animations[0]->Render(x, y, 255);
}

void  BigFire::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (isHitted)
	{
		effect->Update(dt, coObjects);
		if (!isFirst) {
			item = new Item();
			item->AddAnimation(702);
			item->SetPosition(this->dx, this->dy);
			item->SetType(805);
			isFirst = true;
		}
	}
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
