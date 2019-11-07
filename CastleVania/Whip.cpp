
#include "Simon.h"
#include "Whip.h"
#include "BigFire.h"
#include "Effect.h"

void Whip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float wl, wr, wt, wb;
	GetBoundingBox(wl, wt, wr, wb);

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<BigFire *>(coObjects->at(i)))
		{
			BigFire *bigfire = dynamic_cast<BigFire *>(coObjects->at(i));
			float zl, zr, zt, zb;
			bigfire->GetBoundingBox(zl, zt, zr, zb);
			if (wr >zl&&wr< zr )
			{
				bigfire->isHitted = true;
				bigfire->effect->firstRenderTime = GetTickCount();
			}
		}
	}
}

void Whip::Render()
{
	RenderBoundingBox();
}

void Whip::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + WHIP_LV0_BBOX_WIDTH;
	bottom = y + WHIP_LV0_BBOX_HEIGHT;
	
}

Whip::~Whip()
{

}
