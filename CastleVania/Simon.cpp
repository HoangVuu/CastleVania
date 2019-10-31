#include <algorithm>
#include "debug.h"

#include "Simon.h"
#include "Game.h"
#include "BigFire.h"
#include "Ground.h"


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING_RIGHT	100
#define SIMON_STATE_WALKING_LEFT	200
#define SIMON_STATE_JUMP			300
#define SIMON_STATE_DIE				400
#define SIMON_STATE_MOVE			500
#define SIMON_STATE_HIT				600
#define SIMON_STATE_SIT				700

#define SIMON_DIE_DEFLECT_SPEED	 0.5f



#define SIMON_ANI_DIE				8

#define SIMON_UNTOUCHABLE_TIME 5000
#define SIMON_JUMP_SPEED_Y		0.5f
#define SIMON_JUMP_DEFLECT_SPEED 0.2f
#define SIMON_GRAVITY			0.002f

#define SIMON_WALKING_SPEED		0.1f 

#define SIMON_ANI_BIG_IDLE_RIGHT		0
#define SIMON_ANI_BIG_IDLE_LEFT			1

#define SIMON_ANI_BIG_WALKING_RIGHT		2
#define SIMON_ANI_BIG_WALKING_LEFT		3

#define SIMON_ANI_HIT_RIGHT				4
#define SIMON_ANI_HIT_LEFT				5

#define SIMON_ANI_JUMP_RIGHT			6
#define SIMON_ANI_JUMP_LEFT				7

#define SIMON_ANI_SIT_RIGHT				8
#define SIMON_ANI_SIT_LEFT				9

#define SIMON_ANI_SIT_HIT_RIGHT			10
#define SIMON_ANI_SIT_HIT_LEFT			11

#define SIMON_ANI_DIE					12
#define WHIP_HIT						13

//roi
#define WHIP_RIGHT 						0
#define WHIP_LEFT						1

#define	SIMON_LEVEL_SMALL	1
#define	SIMON_LEVEL_BIG		2

#define SIMON_BIG_BBOX_WIDTH  48
#define SIMON_BIG_BBOX_HEIGHT 64

#define SIMON_STAND_BBOX_WIDTH			32
#define SIMON_STAND_BBOX_HEIGHT			60



void Simon::CalcPotentialCollisions(
	vector<LPGAMEOBJECT> *coObjects,
	vector<LPCOLLISIONEVENT> &coEvents)
{
	
	bool isCollideWithCheckBox = false;
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		float cbl, cbr, cbt, cbb;
		coObjects->at(i)->GetBoundingBox(cbl, cbt, cbr, cbb);

		// Simon se khong va cham voi nhung vat sau:
		if (!dynamic_cast<BigFire *>(coObjects->at(i)))		// simon ko va chạm với lửa
		{
			LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
			if (dynamic_cast<Ground *>(coObjects->at(i)))
			{
				if (x < cbr && x + SIMON_STAND_BBOX_WIDTH > cbl && y < cbb && y + SIMON_STAND_BBOX_HEIGHT > cbt)
				{
					isCollideHorizontal = false;
				}
			}
			if (e->t > 0 && e->t <= 1.0f)
				coEvents.push_back(e);
			else
				delete e;
		}
	}
	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	//nếu đang đánh thì không update sự kiện đánh lần nữa.
	if (isAttack == true && GetTickCount() - attackTime >= 450) {
		isAttack = false;
		whip->Update(attackTime, coObjects);
		vx = 0;
	}
	
	// Simple fall down
	vy += SIMON_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}

	else if (x < 0)
		x = 0;
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) {
			vx = 0;
			//isJump = false;
		}
		if (ny != 0) {
			vy = 0;
			isJump = false;
		}
	}
	whip->SetPosition(x, y);
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Simon::Render()
{
	int ani;

	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_DIE;
	else if (isAttack) {
			if (nx > 0) {
				if (isSit)
					ani = SIMON_ANI_SIT_HIT_RIGHT;
				else
					ani = SIMON_ANI_HIT_RIGHT;
			}
			else
			{
				if (isSit)
					ani = SIMON_ANI_SIT_HIT_LEFT;
				else
					ani = SIMON_ANI_HIT_LEFT;
			}
		}
		else if (isJump) {
			if (nx > 0)
				ani = SIMON_ANI_JUMP_RIGHT;
			else
				ani = SIMON_ANI_JUMP_LEFT;
		}
		else
		{
			if (vx == 0) {
				if (!isSit) {
					if (nx > 0)
						ani = SIMON_ANI_BIG_IDLE_RIGHT;
					else
						ani = SIMON_ANI_BIG_IDLE_LEFT;
				}
				else {
					if (nx > 0)
						ani = SIMON_ANI_SIT_RIGHT;
					else
						ani = SIMON_ANI_SIT_LEFT;
				}
			}
			else if (vx > 0)
				ani = SIMON_ANI_BIG_WALKING_RIGHT;
			else
				ani = SIMON_ANI_BIG_WALKING_LEFT;
		}


	int alpha = 255;
	if (untouchable) alpha = 128;


	//animations[ani]->ResetFrame();

	if (isAttack) {
		if (nx > 0) {
			whip->animations[WHIP_RIGHT]->Render(x, y, alpha);
			animations[ani]->Render(x, y, alpha);
		}
		else {
			whip->animations[WHIP_LEFT]->Render(x, y, alpha);
			animations[ani]->Render(x, y, alpha);
		}
	}
	else
		animations[ani]->Render(x, y, alpha);
		RenderBoundingBox();
}

void Simon::SetState(int state)
{
	isSit = false;
	//isRight = true;

	CGameObject::SetState(state);
	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		isSit = false;
		break;
	case SIMON_STATE_WALKING_LEFT:
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		isSit = false;
		break;
	case SIMON_STATE_JUMP:
		if (isJump)return;
		vy = -SIMON_JUMP_SPEED_Y;
		isJump = true;
		isSit = false;
		//isAttack = false;
	case SIMON_STATE_IDLE:
		vx = 0;
		break;
	case SIMON_STATE_DIE:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		isJump = false;
		isSit = false;
		break;
	case SIMON_STATE_HIT:
		if (isAttack)return;
		attackTime = GetTickCount();
		vx = 0;
		isAttack = true;
		break;
	case SIMON_STATE_SIT:
		vx = 0;
		isSit = true;
		break;
	}
	if (vx > 0)
		isRight = true;
	else
		isRight = false;

}
	
void Simon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	right = x + SIMON_BIG_BBOX_WIDTH;
	bottom = y + SIMON_BIG_BBOX_HEIGHT;
}
	