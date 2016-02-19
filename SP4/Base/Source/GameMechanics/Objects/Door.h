#pragma once
#include "Objects.h"

class CDoor : public CObjects
{
private:
	int id;

public:
	CDoor();
	CDoor(int id, Vector3 pos, Vector3 scale, Mesh* mesh);
	~CDoor();

	//set and get the id number of the door
	void setId(int);			
	int getId();

	//check whether player reached the door
	void setReached(bool);
	bool getIfReached();
};