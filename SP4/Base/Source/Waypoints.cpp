#include "Waypoints.h"


CWaypoints::CWaypoints()
{
}


CWaypoints::~CWaypoints()
{
}
void CWaypoints::LoadWaypoints(CMap &map)
{
	for (int i = 0; i < map.GetNumOfTiles_Height(); i++)
	{
		for (int k = 0; k < map.GetNumOfTiles_Width(); k++)
		{
			int m = k;
			int h = i;
			float x = h*map.GetTileSize() - map.GetScreenHeight() * 0.5;
			float y = m*map.GetTileSize() - map.GetScreenWidth() * 0.5;
			
			if (map.theScreenMap[m][h] == 0)
			{
				myWaypointList.push_back(Vector3(x, y, 0));
			}
		}
	}
}