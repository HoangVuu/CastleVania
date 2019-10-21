#include"Map.h"
#include "Textures.h"
#include "Game.h"
#include "Sprites.h"

Map::Map()
{
	tileWidth = TILE_WIDTH;
	tileHeight = TILE_HEIGHT;
}

Map::Map(/*int _cols, int _rows, */LPDIRECT3DTEXTURE9 sprite, int _tileWidth, int _tileHeight)
{
	/*cols = _cols;
	rows = _rows;*/
	tileSet = sprite;
	tileWidth = _tileWidth;
	tileHeight = _tileHeight;

	matrix = new int *[rows];
	for (int i = 0; i < rows; i++)
	{
		matrix[i] = new int[cols];
	}
}

Map::~Map()
{
	if (matrix)
	{
		for (int i = 0; i < rows; i++)
		{
			delete matrix[i];
		}
		delete matrix;
		matrix = nullptr;
	}
}



void Map::LoadMatrixMap(LPCSTR fileSource)
{
	fstream pFile;
	pFile.open(fileSource, fstream::in);

	string lineString;
	int * posSpace = NULL;
	string subString;
	int countLine = 0;
	int i = 0;

	while (pFile.good())
	{
		getline(pFile, lineString);
		if (countLine == 0)
		{
			cols = atoi(lineString.c_str());
		}
		else if (countLine == 1)
		{
			rows = atoi(lineString.c_str());

			matrix = new int *[rows];
			for (int row = 0; row < rows; row++)
			{
				matrix[row] = new int[cols];
			}
		}
		else
		{
			posSpace = new int[cols];
			for (int j = 0; j < cols; j++)
			{
				if (j == 0)
				{
					posSpace[0] = lineString.find(" ", 0);
					subString = lineString.substr(0, posSpace[0]);
					int id = atoi(subString.c_str());
					matrix[i][j] = id;
				}
				else
				{
					posSpace[j] = lineString.find(" ", posSpace[j - 1] + 1);
					subString = lineString.substr(posSpace[j - 1] + 1, posSpace[j] - (posSpace[j - 1] + 1));
					int id = atoi(subString.c_str());
					matrix[i][j] = id;
				}
			}
			i++;
		}
		countLine++;
	}
}

void Map::Draw(float x, float y)
{
	RECT tileRect;
	D3DXVECTOR3 tilePos;
	int cameraWidth = 640;//viewPort->GetCameraWidth();
	int cameraHeight = 320;//viewPort->GetCameraHeight();

	int colStart = (int)x / tileWidth;
	int colEnd = ((int)x + cameraWidth) / tileWidth < cols - 1 ? (x + cameraWidth) / tileWidth : cols - 1;
	int rowStart = (int)y / tileHeight;
	int rowEnd = ((int)y + cameraHeight) / tileHeight < rows - 1 ? (y + cameraHeight) / tileHeight : rows - 1;

	for (int i = rowStart; i <= rowEnd; i++)
	{
		for (int j = colStart; j <= colEnd; j++)
		{
			//tileRect dung de lay ra RECT trong tile set de ve
			tileRect.left = (matrix[i][j] % 16) * tileWidth; // 16 is number of column in tileset
			tileRect.top = (matrix[i][j] / 16) * tileHeight;
			tileRect.right = tileRect.left + tileWidth;
			tileRect.bottom = tileRect.top + tileHeight;

			//tile pos la vi tri de ve tile len camera
			//tilePos = viewPort->ConvertPosInViewPort(D3DXVECTOR3(j * tileWidth, i * tileHeight + 40, 0));
			CGame::GetInstance()->Draw(tileWidth*j, tileHeight*i + 120, tileSet, tileRect.left, tileRect.top, tileRect.right, tileRect.bottom);

			//tileSet->Draw(x, y, 255); //cast pos to int-type to avoid tearing tilemap
		}
	}
}
