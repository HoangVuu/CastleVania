#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include "GameObject.h"
#include "Sprites.h"


class Whip :public CGameObject
{
	int step = 0;

public:
	int level = 0;
	Whip() :CGameObject()
	{

	}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	/*void UpLevel() { level += 1; }*/
	~Whip();
};

