#include <d3d9.h>
#include <Windows.h>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include"Map.h"
#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Simon.h"
#include "Ground.h"	
#include"BigFire.h"


#define SIMON_WALKING_SPEED				0.1f 

#define SIMON_JUMP_SPEED_Y				0.5f
#define SIMON_JUMP_DEFLECT_SPEED		0.2f
#define SIMON_GRAVITY					0.002f
#define SIMON_DIE_DEFLECT_SPEED			0.5f

#define SIMON_STATE_IDLE			    0
#define SIMON_STATE_WALKING_RIGHT	    100
#define SIMON_STATE_WALKING_LEFT	    200
#define SIMON_STATE_JUMP			    300
#define SIMON_STATE_DIE				    400
#define SIMON_STATE_MOVE			    500
#define SIMON_STATE_HIT				    600
#define SIMON_STATE_SIT				    700

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

//#define SIMON_ANI_BIG_IDLE_RIGHT		0
//#define SIMON_ANI_BIG_IDLE_LEFT			1
//#define SIMON_ANI_HIT_RIGHT				2
//#define SIMON_ANI_HIT_LEFT				3
//#define SIMON_ANI_BIG_WALKING_RIGHT		4
//#define SIMON_ANI_BIG_WALKING_LEFT		5
//#define SIMON_ANI_SIT_RIGHT				6
//#define SIMON_ANI_SIT_LEFT				7
//#define SIMON_ANI_JUMP_RIGHT			8
//#define SIMON_ANI_JUMP_LEFT				9
//#define SIMON_ANI_SIT_HIT_RIGHT			10
//#define SIMON_ANI_SIT_HIT_LEFT			11
//#define SIMON_ANI_DIE					12


#define	SIMON_LEVEL_SMALL	1
#define	SIMON_LEVEL_BIG		2

#define SIMON_BIG_BBOX_WIDTH  15
#define SIMON_BIG_BBOX_HEIGHT 60

#define SIMON_SMALL_BBOX_WIDTH  13
#define SIMON_SMALL_BBOX_HEIGHT 15

#define SIMON_UNTOUCHABLE_TIME 5000

#define WINDOW_CLASS_NAME L"Castlevania"
#define MAIN_WINDOW_TITLE L"Castlevania"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 200)
#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	580
#define MAX_WIDTH_LV1	1536

#define MAX_FRAME_RATE 120

#define ID_TEX_SIMON	0
#define ID_TEX_SIMON_2	1
#define ID_TEX_LV1		2
#define ID_TEX_BRICK	3
#define ID_TEX_FIRE		4
#define ID_TEX_WHIP		5
#define ID_TEX_WHIP_2	6
#define ID_TEX_TILESET	7



CGame *game;

Simon * simon;

Map *map;

CSprite *sprite;

Whip *whip;

DWORD x1;
DWORD x2;
vector<LPGAMEOBJECT> objects;

class CSampleKeyHander : public CKeyEventHandler
{
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander * keyHandler;

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		simon->SetState(SIMON_STATE_JUMP);
		break;
	case DIK_B: // reset
		simon->SetState(SIMON_STATE_IDLE);
		/*simon->SetLevel(SIMON_LEVEL_BIG);*/
		simon->SetPosition(50.0f, 0.0f);
		simon->SetSpeed(0, 0);
		break;
	case DIK_A:
		simon->SetState(SIMON_STATE_HIT);
		break;
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}
bool first = false;
void CSampleKeyHander::KeyState(BYTE *states)
{
	OutputDebugString(L"asdasd");
	//DebugOut(L"tic", x2);
	if (simon->GetState() == SIMON_STATE_DIE)
		return;
	if (game->IsKeyDown(DIK_RIGHT))
		simon->SetState(SIMON_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		simon->SetState(SIMON_STATE_WALKING_LEFT);
	else if (game->IsKeyDown(DIK_DOWN))
		simon->SetState(SIMON_STATE_SIT);
	else if (game->IsKeyDown(DIK_A))
		simon->SetState(SIMON_STATE_HIT);
	else
		simon->SetState(SIMON_STATE_IDLE);
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources
	In this example: load textures, sprites, animations and mario object
	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_SIMON, L"Castlevania\\Simon_right.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_SIMON_2, L"Castlevania\\Simon_ver_editted.png", D3DCOLOR_XRGB(255, 0, 255));

	textures->Add(ID_TEX_LV1, L"Castlevania\\lv1.png", D3DCOLOR_XRGB(176, 224, 248));

	textures->Add(ID_TEX_BRICK, L"Castlevania\\BRICK1.png", D3DCOLOR_XRGB(3, 26, 110));

	textures->Add(ID_TEX_FIRE, L"Castlevania\\123.png", D3DCOLOR_XRGB(255, 0, 255));

	textures->Add(ID_TEX_WHIP, L"Castlevania\\WHIP.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_WHIP_2, L"Castlevania\\WHIP_left.png", D3DCOLOR_XRGB(255, 0, 255));

	textures->Add(ID_TEX_TILESET, L"Castlevania\\tileset.png", D3DCOLOR_XRGB(255, 0, 255));


	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);

	sprites->Add(10001, 436, 0, 484, 64, texSimon);			//đứng im phải

	sprites->Add(10002, 260, 0, 308, 64, texSimon);			//đi phải
	sprites->Add(10003, 314, 0, 362, 64, texSimon);
	sprites->Add(11002, 378, 0, 426, 64, texSimon);
	sprites->Add(11003, 436, 0, 484, 64, texSimon);

	sprites->Add(10004, 120, 0, 168, 64, texSimon);			//đánh phải			
	sprites->Add(10005, 60, 0, 108, 64, texSimon);
	sprites->Add(10006, 0, 0, 64, 64, texSimon);

	sprites->Add(10020, 196, 0, 230, 64, texSimon);			//nhảy phải

	sprites->Add(10018, 196, 17, 230, 64, texSimon);		//ngồi phải

	sprites->Add(10028, 0, 67, 49, 130, texSimon);			//ngồi đánh phải			
	sprites->Add(10029, 420, 134, 469, 197, texSimon);
	sprites->Add(10030, 360, 134, 421, 197, texSimon);

	LPDIRECT3DTEXTURE9 texSimon2 = textures->Get(ID_TEX_SIMON_2);

	sprites->Add(10011, 12, 0, 44, 64, texSimon2);			//đứng im trái

	sprites->Add(10012, 196, 0, 244, 64, texSimon2);		//đi trái
	sprites->Add(10013, 136, 0, 184, 64, texSimon2);
	sprites->Add(11012, 78, 0, 126, 64, texSimon2);
	sprites->Add(11013, 12, 0, 60, 64, texSimon2);

	sprites->Add(10014, 301, 0, 360, 64, texSimon2);		//đánh trái				
	sprites->Add(10015, 361, 0, 404, 64, texSimon2);
	sprites->Add(10016, 421, 0, 464, 64, texSimon2);

	sprites->Add(10021, 256, 0, 286, 64, texSimon2);		//nhảy trái

	sprites->Add(10019, 254, 17, 286, 64, texSimon2);		//ngồi trái

	sprites->Add(10031, 420, 67, 477, 130, texSimon2);		// ngồi đánh trái				
	sprites->Add(10032, 0, 134, 44, 197, texSimon2);
	sprites->Add(10033, 60, 134, 104, 197, texSimon2);


	LPDIRECT3DTEXTURE9 whipR = textures->Get(ID_TEX_WHIP);
	sprites->Add(10056, 570, 0, 553, 67, whipR);			//roi lv0 phải	
	sprites->Add(10057, 346, 0, 312, 49, whipR);
	sprites->Add(10058, 114, 0, 188, 30, whipR);

	LPDIRECT3DTEXTURE9 whipL = textures->Get(ID_TEX_WHIP_2);
	sprites->Add(10059, 110, 0, 166, 66, whipL);			//roi lv0 trái
	sprites->Add(10060, 349, 0, 408, 52, whipL);
	sprites->Add(10061, 585, 0, 529, 30, whipL);

	sprites->Add(10099, 180, 237, 240, 264, texSimon);		// chết 

	LPDIRECT3DTEXTURE9 texMisc = textures->Get(ID_TEX_BRICK);
	sprites->Add(20001, 0, 0, 32, 32, texMisc);

	//LPDIRECT3DTEXTURE9 texEnemy = textures->Get(ID_TEX_ZOMBIE);
	//sprites->Add(30001, 0, 0, 32, 64, texEnemy); // đi trái 
	//sprites->Add(30002, 32, 0, 68, 64, texEnemy);

	//LPDIRECT3DTEXTURE9 texEnemy1 = textures->Get(ID_TEX_PANTHER);
	//sprites->Add(30011, 0, 0, 50, 32, texEnemy1); // nằm chờ
	//sprites->Add(30012, 64, 0, 120, 32, texEnemy1);
	//sprites->Add(30013, 126, 0, 188, 32, texEnemy1);
	//sprites->Add(30014, 188, 0, 256, 32, texEnemy1);

	LPDIRECT3DTEXTURE9 texEnemy2 = textures->Get(ID_TEX_FIRE);
	sprites->Add(40011, 0, 0, 32, 64, texEnemy2);
	sprites->Add(40012, 32, 0, 64, 64, texEnemy2);


	LPANIMATION ani;
	{
		ani = new CAnimation(100);	//đứng phải
		ani->Add(10001);
		animations->Add(400, ani);

		ani = new CAnimation(100);	//đứng trái
		ani->Add(10011);
		animations->Add(401, ani);

		ani = new CAnimation(150);	//đánh phải
		ani->Add(10004);
		ani->Add(10005);
		ani->Add(10006);
		animations->Add(402, ani);

		ani = new CAnimation(150);//đánh trái
		ani->Add(10014);
		ani->Add(10015);
		ani->Add(10016);
		animations->Add(403, ani);

		ani = new CAnimation(180);	//đi phải
		ani->Add(10002);
		ani->Add(10003);
		ani->Add(11002);
		ani->Add(11003);
		animations->Add(500, ani);

		ani = new CAnimation(180);	//đi trái
		ani->Add(10012);
		ani->Add(10013);
		ani->Add(11012);
		ani->Add(11013);
		animations->Add(501, ani);

		ani = new CAnimation(100);	//nhảy phải
		ani->Add(10020);
		animations->Add(404, ani);

		ani = new CAnimation(100);	//nhảy trái
		ani->Add(10021);
		animations->Add(405, ani);

		ani = new CAnimation(100);	//ngồi phải
		ani->Add(10018);
		animations->Add(406, ani);

		ani = new CAnimation(100);//ngồi trái
		ani->Add(10019);
		animations->Add(407, ani);

		ani = new CAnimation(150);//ngồi đánh phải
		ani->Add(10028);
		ani->Add(10029);
		ani->Add(10030);
		animations->Add(408, ani);

		ani = new CAnimation(150);//ngồi đánh trái
		ani->Add(10031);
		ani->Add(10032);
		ani->Add(10033);
		animations->Add(409, ani);

		ani = new CAnimation(150);//roi phải 
		ani->Add(10056);
		ani->Add(10057);
		ani->Add(10058);
		animations->Add(602, ani);

		ani = new CAnimation(150);//roi trái
		ani->Add(10059);
		ani->Add(10060);
		ani->Add(10061);
		animations->Add(603, ani);


		ani = new CAnimation(100);//CHET
		ani->Add(10099);
		animations->Add(599, ani);

		ani = new CAnimation(100);// GACH
		ani->Add(20001);
		animations->Add(601, ani);

		/*ani = new CAnimation(100);
		ani->Add(30001);
		ani->Add(30002);
		animations->Add(602, ani);
		*/
	}
	ani = new CAnimation(100);
	ani->Add(40011);
	ani->Add(40012);
	animations->Add(700, ani);
	simon = new Simon();
	{
		simon->AddAnimation(400);			//đứng phải
		simon->AddAnimation(401);			//đứng trái

		simon->AddAnimation(500);			//đi phải
		simon->AddAnimation(501);			//đi trái

		simon->AddAnimation(402);			//đánh phải 
		simon->AddAnimation(403);			//đánh trái

		simon->AddAnimation(404);			//nhảy phải 
		simon->AddAnimation(405);			//nhảy trái

		simon->AddAnimation(406);			//ngồi phải 
		simon->AddAnimation(407);			//ngồi trái

		simon->AddAnimation(408);			//ngồi đánh phải
		simon->AddAnimation(409);			//ngồi đánh trái

		simon->AddAnimation(599);			//chết

		simon->whip->AddAnimation(602);		//roi lv0 phải
		simon->whip->AddAnimation(603);		//roi lv0 trái

	}
	simon->SetPosition(150, 327);
	//	whip->AddAnimation(408);// roi
	objects.push_back(simon);
	for (int i = 0; i < 50; i++)
	{
		Ground *ground = new Ground();
		ground->AddAnimation(601);
		ground->SetPosition(0 + i * 16.0f, 407);
		objects.push_back(ground);
	}

	BigFire *bigfire = new BigFire();
	bigfire->AddAnimation(700);
	bigfire->SetPosition(350, 350);
	objects.push_back(bigfire);
	BigFire *bigfire1 = new BigFire();
	bigfire1->AddAnimation(700);
	bigfire1->SetPosition(481, 350);
	BigFire *bigfire2 = new BigFire();
	bigfire2->AddAnimation(700);
	bigfire2->SetPosition(672, 350);
	objects.push_back(bigfire2);
	BigFire *bigfire3 = new BigFire();
	bigfire3->AddAnimation(700);
	bigfire3->SetPosition(866, 350);
	objects.push_back(bigfire3);
	BigFire *bigfire4 = new BigFire();
	bigfire4->AddAnimation(700);
	bigfire4->SetPosition(1104, 350);
	objects.push_back(bigfire4);
	BigFire *bigfire5 = new BigFire();
	bigfire5->AddAnimation(700);
	bigfire5->SetPosition(1282, 350);
	objects.push_back(bigfire5);

	LPDIRECT3DTEXTURE9 tileset1 = textures->Get(ID_TEX_TILESET);
	//sprite = new CSprite(500000, 0, 0, 256, 64, tileset1);
	map = new	Map(tileset1, 32, 32);
	map->LoadMatrixMap("Castlevania\\Mapstate.txt");
	//map->Draw(game->x_cam, game->y_cam);
}


void Update(DWORD dt)
{

	vector<LPGAMEOBJECT> coObjects;
	for (int i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	float x, y;
	simon->GetPosition(x, y);

	//Camera follow Simon
	if (x > SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV1 - SCREEN_WIDTH / 2)
	{
		game->x_cam = x - SCREEN_WIDTH / 2;

	}
	else if (x > MAX_WIDTH_LV1 - SCREEN_WIDTH / 2)
	{
		game->x_cam = MAX_WIDTH_LV1 - SCREEN_WIDTH;

	}
	else
	{
		game->x_cam = 0;
	}
}


void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);
		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
		map->Draw(game->x_cam, game->y_cam);

		for (int i = 0; i < objects.size(); i++)
			objects[i]->Render();

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow	 failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;
	//GameTime *time = new GameTime;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();

			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);

	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	//x1 = GetTickCount();
	Run();

	return 0;
}