#include "Effect.h"


void Effect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount() - firstRenderTime > EFFECT_TIME)
		isExposed = true;
}

void Effect::Render()
{
	animations[0]->Render(x, y);
	//RenderBoundingBox();
}

void Effect::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = 0;
	t = 0;
	r = 0;
	b = 0;
}

