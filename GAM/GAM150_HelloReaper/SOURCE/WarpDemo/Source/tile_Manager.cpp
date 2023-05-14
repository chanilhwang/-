/******************************************************************************
FileName: tile_Manager.cpp
Project: Hello Reaper
Author: Yongwon, Cho (y.cho)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "tile_Manager.h"

namespace 
{
	#define TILE_OFFSET_WIDTH 15.8f
	#define TILE_COLLIDE_SIZE 0.3f
	#define TILE_SIZE 0.65f
}

	void tile_manager::DestroyTiles()
    {
        delete[] DeathTiles;
        delete[] MoveTiles;
    }
    
	void tile_manager::DrawTiles(WEMtx44& _transform, int DeathTileTxt, 
		int MoveTileLeftTxt, int MoveTileRightTxt, 
		int MoveTileUpTxt, int MoveTileDownTxt,
		point Offset)
    {
		//Draw DeathTiles
		for (int Index = 0; Index < NumDeath; Index++)
		{
				//Draw 
				WEGraphics::SetTexture(DeathTileTxt);
				//WEGraphics::SetTexture(DeathTileTxt);
			if (DeathTiles[Index].State)
				WEGraphics::SetTextureColor(0xffe545f7);
			else
				WEGraphics::SetTextureColor(0xffffffff);
				WEMtx44::MakeTransform(_transform, DeathTiles[Index].Width *TILE_SIZE, DeathTiles[Index].Height*TILE_SIZE,
					0.0f,
					DeathTiles[Index].Pos.X, DeathTiles[Index].Pos.Y,
					0.0f);
				WEGraphics::Draw(_transform);
		}


		//Draw MoveTiles
		for (int Index = 0; Index < NumMove; Index++)
		{
				//Draw
			switch (MoveTiles[Index].TypeID)
			{
			case 3:
				WEGraphics::SetTexture(MoveTileLeftTxt);
				break;
			case 4:
				WEGraphics::SetTexture(MoveTileRightTxt);
				break;
			case 5:
				WEGraphics::SetTexture(MoveTileUpTxt);
				break;
			case 6:
				WEGraphics::SetTexture(MoveTileDownTxt);
				break;
			}
				
			if (MoveTiles[Index].State)
				WEGraphics::SetTextureColor(0xff57e3b3);
			else
				WEGraphics::SetTextureColor(0xffffffff);
				WEMtx44::MakeTransform(_transform, MoveTiles[Index].Width*(TILE_SIZE-0.10f), MoveTiles[Index].Height*TILE_SIZE,
					0.0f,
					MoveTiles[Index].Pos.X, MoveTiles[Index].Pos.Y,
					0.0f);
				WEGraphics::Draw(_transform);
		}
    }