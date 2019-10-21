#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#pragma once
#include <fstream>
#include <sstream>
#include <d3dx9.h>
#include <string.h>
#include "Sprites.h"
#include"Game.h"

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

class Map
{
private:
	int ** matrix; // map data
	int cols, rows; // cols and rows of our map
	int tileWidth, tileHeight;

	LPDIRECT3DTEXTURE9 tileSet; //

public:
	Map();
	Map(/*int _cols, int _rows,*/ LPDIRECT3DTEXTURE9 sprite, int _tileWidth, int _tileHeight);
	~Map();
	void LoadMatrixMap(LPCSTR fileSource);
	void Draw(float x, float y);

	int GetColumns() { return this->cols; }
	int GetRows() { return this->rows; }
	int GetTileWidth() { return this->tileWidth; }
	int GetTileHeight() { return this->tileHeight; }
};

