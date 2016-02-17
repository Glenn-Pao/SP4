#include "PathFinding.h"

PathFinding::PathFinding(void)
{
}

vector<Cell*> PathFinding::FindPath(Cell* Start, Cell* Destination, CMap* Map)
{
	ValidRoute.clear();
	OpenList.clear();
	ClosedList.clear();

	InitialiseSNDCell(Start, Destination);

	//Current starts as first Node
	OpenList.push_back(Current);

	//While Current != Destination
	while (!(Current->getTileX() == End->getTileX() && Current->getTileY() == End->getTileY())
		&& OpenList.size() != 0)
	{
		Current = OpenList[0];
		for (int i = 0; i < OpenList.size(); ++i)
		{
			if (OpenList[i]->getF() <= Current->getF())
			{
				Current = OpenList[i];
			}
		}

		OpenList.erase(std::remove(OpenList.begin(), OpenList.end(), Current), OpenList.end());

		ClosedList.push_back(Current);

		for (int x = -1; x < 2; ++x)
		{
			for (int y = -1; y < 2; ++y)
			{

				if (abs(x) != abs(y))
				{
					if (Map->theScreenMap[Current->getTileY() + y][Current->getTileX() + x] != 1)
					{
						Cell* Test = new Cell(Current->getTileX() + x, Current->getTileY() + y);

						if (isCellinVector(Test, ClosedList) != true)
						{
							if (isCellinVector(Test, OpenList) == true)
							{
								UpdateOpenList(Test, Current);
							}
							else
							{
								Test->setG(Current->G + 1);
								Test->setH(Destination);
								Test->setParent(Current);
								OpenList.push_back(Test);
							}
						}
					}
				}
			}
		}
	}

	while (Current->getParent() != 0)
	{
		ValidRoute.push_back(Current);
		Current = Current->getParent();
	}
	return ValidRoute;
}

int PathFinding::UpdateOpenList(Cell* CellToUpdate, Cell* CellToUse)
{
	for (int i = 0; i < OpenList.size(); ++i)
	{
		if (OpenList[i] == CellToUpdate)
		{
			if ((Current->getG() + 1) < OpenList[i]->getG())
			{
				OpenList[i]->setParent(CellToUse);
				OpenList[i]->setG(CellToUse->getG() + 1);
				return i;
			}
		}
	}

}

bool PathFinding::isCellinVector(Cell* CellToCheck, vector<Cell*> VectorToCheck)
{
	for (int i = 0; i < VectorToCheck.size(); ++i)
	{
		if (VectorToCheck[i]->operator==(CellToCheck))
		{
			return true;
		}
	}
	return false;
}

void PathFinding::InitialiseSNDCell(Cell* Start, Cell* Destination)
{
	Start->setParent(0);
	Start->setTileX(Start->getTileX() / 32);
	Start->setTileY(Start->getTileY() / 32);
	Start->setG(0);

	Destination->setTileX(Destination->getTileX() / 32);
	Destination->setTileY(Destination->getTileY() / 32);

	Start->setH(Destination);

	this->Current = Start;
	this->End = Destination;
}

PathFinding::~PathFinding(void)
{
}