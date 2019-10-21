#include <algorithm>
#include "debug.h"

#include "Simon.h"
#include "Game.h"
#include "BigFire.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 580

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

#define SIMON_ANI_BIG_WALKING_RIGHT			2
#define SIMON_ANI_BIG_WALKING_LEFT			3

#define SIMON_ANI_HIT_RIGHT				4
#define SIMON_ANI_HIT_LEFT				5

#define SIMON_ANI_JUMP_RIGHT			6
#define SIMON_ANI_JUMP_LEFT				7

#define SIMON_ANI_SIT_RIGHT				8
#define SIMON_ANI_SIT_LEFT				9
#define SIMON_ANI_DIE					10



#define	SIMON_LEVEL_SMALL	1
#define	SIMON_LEVEL_BIG		2

#define SIMON_BIG_BBOX_WIDTH  35
#define SIMON_BIG_BBOX_HEIGHT 64

#define SIMON_SMALL_BBOX_WIDTH  35
#define SIMON_SMALL_BBOX_HEIGHT 64

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

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

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Simon::Render()
{
	int ani;

	if (vy < 0 && state == SIMON_STATE_JUMP)
	{

		if (nx < 0)
			ani = SIMON_ANI_JUMP_RIGHT;
		ani = SIMON_ANI_JUMP_LEFT;
	}

	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_DIE;
	else
		if (level == SIMON_LEVEL_BIG)
		{// bawts may mess
			if (vx == 0) // nge chua
			{
				if (isSit&&isRight)
					ani = SIMON_ANI_SIT_RIGHT;
				else if (isSit&&isRight==false) 
					ani = SIMON_ANI_SIT_LEFT;
				else if (nx > 0 && state != SIMON_STATE_HIT) {
					ani = SIMON_ANI_BIG_IDLE_RIGHT;
					if (isJump)
						ani = SIMON_ANI_JUMP_RIGHT;
				}
				else if (nx < 0 && state != SIMON_STATE_HIT) {
					ani = SIMON_ANI_BIG_IDLE_LEFT;
					if (isJump)
						ani = SIMON_ANI_JUMP_LEFT;
				}
				else if (nx > 0 && state == SIMON_STATE_HIT) {
					ani = SIMON_ANI_HIT_RIGHT;
				}
				else if (nx < 0 && state == SIMON_STATE_HIT) {
					ani = SIMON_ANI_HIT_LEFT;
				}
				else if (nx > 0 && state == SIMON_STATE_SIT)
					ani = SIMON_ANI_SIT_RIGHT;
				else if (nx < 0 && state == SIMON_STATE_SIT)
					ani = SIMON_ANI_SIT_LEFT;

			}

			else if (vx > 0 && !isJump)
				ani = SIMON_ANI_BIG_WALKING_RIGHT;
			else if (vx > 0 && isJump) {
				ani = SIMON_ANI_JUMP_RIGHT;
				if (isAttack)
					ani = SIMON_ANI_HIT_RIGHT;
			}
			else if (vx < 0 && !isJump)
				ani = SIMON_ANI_BIG_WALKING_LEFT;
			else if (vx < 0 && isJump) {
				ani = SIMON_ANI_JUMP_LEFT;
				if (isAttack)
					ani = SIMON_ANI_HIT_LEFT;
			}
		}



	int alpha = 255;
	if (untouchable) alpha = 128;

	animations[ani]->Render(x, y, alpha);
	RenderBoundingBox();
}

	void Simon::SetState(int state)
	{
		CGameObject::SetState(state);
		isSit=false;
		switch (state)
		{
		case SIMON_STATE_WALKING_RIGHT:
			vx = SIMON_WALKING_SPEED;
			nx = 1;
			break;
		case SIMON_STATE_WALKING_LEFT:
			vx = -SIMON_WALKING_SPEED;
			nx = -1;
			break;
		case SIMON_STATE_JUMP: {
			vy = -SIMON_JUMP_SPEED_Y;
			isJump = true;
		}
		case SIMON_STATE_IDLE: {
			vx = 0;
			break;
			isJump = true;
			isAttack = true;
		}
		case SIMON_STATE_DIE:
			vy = -SIMON_DIE_DEFLECT_SPEED;
			isJump = false;
			break;
			/*case SIMON_STATE_MOVE:
				vx = SIMON_WALKING_SPEED;
				nx = 1;
				break;*/
		case SIMON_STATE_HIT :
			vx = 0;
			isJump = true;
			isAttack = true;
			break;
		case SIMON_STATE_SIT :
			vx = 0;
			isSit = true;
			break;
		}
	}
void Simon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	if (level == SIMON_LEVEL_BIG)
	{
		right = x + SIMON_BIG_BBOX_WIDTH;
		bottom = y + SIMON_BIG_BBOX_HEIGHT;
	}
	else
	{
		right = x + SIMON_SMALL_BBOX_WIDTH;
		bottom = y + SIMON_SMALL_BBOX_HEIGHT;
	}
}