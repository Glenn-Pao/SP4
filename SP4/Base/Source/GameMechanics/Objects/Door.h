#pragma once
#include "Objects.h"
#include "BoundingBox.h"

class CDoor : public CObjects
{
private:
	CBoundingBox* CollideBox;
	int id;

public:
	CDoor();
	CDoor(int id, Vector3 pos, Vector3 scale, Mesh* mesh);
	~CDoor();

	void setId(int);
	int getId();

	void setReached(bool);
	bool getIfReached();

	CBoundingBox* getCollideBox();
};