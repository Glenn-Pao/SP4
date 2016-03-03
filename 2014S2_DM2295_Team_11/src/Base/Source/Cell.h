#include "Vector3.h"
#include <math.h>

struct Cell
{
	int TileX;
	int TileY;

	Cell* Parent;

	int G;
	int H;
	int F;

	bool operator ==(Cell* CellToCompare)
	{
		if (this->getTileX() == CellToCompare->getTileX() && this->getTileY() == CellToCompare->getTileY())
		{
			return true;
		}
		return false;
	}

	void setParent(Cell* Parent)
	{
		this->Parent = Parent;
	}

	void setTileX(int X)
	{
		this->TileX = X;
	}

	void setTileY(int Y)
	{
		this->TileY = Y;
	}

	void setG(int G)
	{
		this->G = G;
	}


	void setH(Cell* Last)
	{
		this->H = abs(Last->getTileX() - this->getTileX()) + abs(Last->getTileY() - this->getTileY());
	}

	Cell* getParent()
	{
		return Parent;
	}

	int getTileX()
	{
		return TileX;
	}

	int getTileY()
	{
		return TileY;
	}

	int getG()
	{
		return G;
	}

	int getH()
	{
		return H;
	}
	int getF()
	{
		return G + H;
	}

	Cell() : Parent(0)
	{
	}

	Cell(int x, int y, Cell *Parent = NULL) : TileX(x), TileY(y), Parent(Parent), G(0), H(0) {}

	~Cell(){}
};
