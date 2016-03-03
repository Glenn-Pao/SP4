#pragma once

#include <cmath>
#include <vector>
#include <algorithm>
#include "Cell.h"
#include "Map.h"

class PathFinding
{
private:

	Cell* Current;
	Cell* End;

	vector<Cell*> OpenList;
	vector<Cell*> ClosedList;

	vector<Cell*> ValidRoute;
public:

	vector<Cell*> FindPath(Cell* Start, Cell* Destination, CMap* Map);

	void InitialiseSNDCell(Cell* Start, Cell* Destination);

	bool isCellinVector(Cell* CellToCheck, vector<Cell*> VectorToCheck);

	int UpdateOpenList(Cell* CellToUpdate, Cell* CellToUse);
	//void InitialiseOpenList(vector<Map*>* level_map);


	PathFinding(void);
	~PathFinding(void);
};