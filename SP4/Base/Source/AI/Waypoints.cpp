#include "Waypoints.h"


CWaypoints::CWaypoints()
{
}


CWaypoints::~CWaypoints()
{
}

//load the waypoints based on the map dimensions
void CWaypoints::LoadWaypoints(CMap *map)
{
	for (int i = 0; i < map->getNumOfTiles_MapHeight(); i++)
	{
		for (int k = 0; k < map->getNumOfTiles_MapWidth(); k++)
		{
			//for now a sweeping method to set up waypoints
			if (map->theScreenMap[i][k] != 1)
			{
				//find the x and y value based on map data
				float x = k * map->GetTileSize();
				float y = map->GetScreenHeight() - i * map->GetTileSize() - map->GetTileSize();
				//put the x and y values into the waypoint list
				myWaypointList.push_back(Vector3(x, y, 0));
			}
		}
	}
	std::cout << "Waypoints generated!" << std::endl;
}
