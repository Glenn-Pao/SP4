#pragma once
#include <vector>
#include "Vector3.h"
#include "Map.h"

/* 
A class to contain the waypoints to be used on the various maps later on.

Done by Almeda Glenn Paolo Maestro
*/
class CWaypoints
{
private:
	std::vector<Vector3>myWaypointList;
public:
	CWaypoints();
	~CWaypoints();

	void LoadWaypoints(CMap *map);		//get a reference of the map

	const std::vector<Vector3> getWaypointsVector()
	{
		return myWaypointList;
	}
};