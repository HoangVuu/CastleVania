#include "Item.h"
#include <algorithm>

void Item::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (!dynamic_cast<BigFire *>(coObjects->at(i)) &&	
			!dynamic_cast<Item *>(coObjects->at(i)) )
		{
			LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

			if (e->t > 0 && e->t <= 1.0f)
				coEvents.push_back(e);
			else
				delete e;
		}
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CalcPotentialCollisions(coObjects, coEvents);

	vy += SIMON_GRAVITY * dt;

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
	}
}

void Item::Render()
{
	animations[0]->Render(x, y);
}

void Item::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	switch (type)
	{
	case ITEM_HEART:
		r = x + HEART_BBOX_WIDTH;
		b = y + HEART_BBOX_HEIGHT;
		break;
	case ITEM_WHIPITEM:
		r = x + WHIPITEM_BBOX_WIDTH;
		b = y + WHIPITEM_BBOX_HEIGHT;
		break;
	case ITEM_KNIFE:
		r = x + KNIFE_BBOX_WIDTH;
		b = y + KNIFE_BBOX_HEIGHT;
		break;
	}
}


