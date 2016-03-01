#include "SceneGameBase.h"

extern "C" {
#include "..\..\Lua\lua.h"
#include "..\..\Lua\lualib.h"
#include "..\..\Lua\lauxlib.h"
}

CSceneGameBase::CSceneGameBase()
	: m_cMinimap(NULL)
	, m_cMap(NULL)
	, tileOffset_x(0)
	, tileOffset_y(0)
	, m_cRearMap(NULL)
	, rearWallOffset_x(0)
	, rearWallOffset_y(0)
	, rearWallTileOffset_x(0)
	, rearWallTileOffset_y(0)
	, rearWallFineOffset_x(0)
	, rearWallFineOffset_y(0)
	, noOfJellybeans(0)
	, theHero(NULL)
	, waypoints(NULL)
{
}
CSceneGameBase::CSceneGameBase(const int m_window_width, const int m_window_height)
	: m_cMinimap(NULL)
	, m_cMap(NULL)
	, tileOffset_x(0)
	, tileOffset_y(0)
	, m_cRearMap(NULL)
	, rearWallOffset_x(0)
	, rearWallOffset_y(0)
	, rearWallTileOffset_x(0)
	, rearWallTileOffset_y(0)
	, rearWallFineOffset_x(0)
	, rearWallFineOffset_y(0)
	, noOfJellybeans(0)
	, theHero(NULL)
	, waypoints(NULL)
{
	sceneManager2D.m_window_width = m_window_width;
	sceneManager2D.m_window_height = m_window_height;
}

CSceneGameBase::~CSceneGameBase()
{
	//for (int i = 0; i<10; i++)
	//{
	//	//delete theArrayOfGoodies[i];
	//}
	//delete theArrayOfGoodies;
	for (int i = 0; i < theEnemies.size(); i++)
	{
		if (theEnemies[i])
		{
			delete theEnemies[i];
			theEnemies[i] = NULL;
		}
	}

	if (m_cMap)
	{
		delete m_cMap;
		m_cMap = NULL;
	}

	if (m_cMinimap)
	{
		delete m_cMinimap;
		m_cMinimap = NULL;
	}
	if (waypoints)
	{
		delete waypoints;
		waypoints = NULL;
	}
}
// Find and Set the actual offset of hero
void CSceneGameBase::SetHeroOffset()
{
	// X
	int centerBorderX = (m_cMap->GetNumOfTiles_Width() * 0.5) * m_cMap->GetTileSize();
	if (theHero->getPositionX() < centerBorderX)
	{
		theHero->SetMapOffset_x(theHero->getPositionX() - centerBorderX);
		if (theHero->GetMapOffset_x() < 0)
		{
			theHero->SetMapOffset_x(0);
		}
		else
		{
			//theHero->setPositionX(centerBorderX);
		}
	}
	else
	{
		theHero->SetMapOffset_x(theHero->getPositionX() - centerBorderX);
		float maxMapOffset_x = (m_cMap->getNumOfTiles_MapWidth() - m_cMap->GetNumOfTiles_Width()) * m_cMap->GetTileSize();
		if (theHero->GetMapOffset_x() > maxMapOffset_x)
		{
			theHero->setPositionX(centerBorderX + theHero->GetMapOffset_x() - maxMapOffset_x);
			theHero->SetMapOffset_x(maxMapOffset_x);
		}
		else
		{
			//theHero->setPositionX(centerBorderX);
		}
	}
	// Y
	int centerBorderY = (m_cMap->GetNumOfTiles_Height() * 0.5) * m_cMap->GetTileSize();
	if (theHero->getPositionY() < centerBorderY)
	{
		theHero->SetMapOffset_y(centerBorderY - theHero->getPositionY() + m_cMap->GetTileSize());
		float maxMapOffset_y = (m_cMap->getNumOfTiles_MapHeight() - m_cMap->GetNumOfTiles_Height()) * m_cMap->GetTileSize();
		if (theHero->GetMapOffset_y() > maxMapOffset_y)
		{
			//theHero->setPositionY(centerBorderY - (theHero->GetMapOffset_y() - maxMapOffset_y));
			theHero->SetMapOffset_y(maxMapOffset_y);
		}
		else
		{
			//theHero->setPositionY(centerBorderY);
		}
	}
	else
	{
		theHero->SetMapOffset_y(centerBorderY - theHero->getPositionY());
		if (theHero->GetMapOffset_y() < m_cMap->GetTileSize())
		{
			//theHero->setPositionY(centerBorderY - (theHero->GetMapOffset_y()));
			theHero->SetMapOffset_y(m_cMap->GetTileSize());
		}
		else
		{
			//theHero->setPositionY(centerBorderY);
		}
	}
}