#pragma once
#include "Objects.h"

class CColour : public CObjects
{
private:
	std::string colour;

public:
	CColour();
	CColour(std::string colour, Vector3 pos, Vector3 scale, Mesh* mesh);
	CColour(std::string colour);
	~CColour();

	void setColour(std::string colour);
	std::string getColour();
};

class CDoor : public CObjects
{
private:
	int id;
	std::string DoorType;
	CColour* theColour;
	

public:
	CDoor();
	CDoor(int id, Vector3 pos, Vector3 scale, Mesh* mesh);
	~CDoor();

	//set and get the id number of the door
	void setId(int);			
	int getId();

	//set and get the door type (normal or coloured(for puzzle))
	void setDoorType(std::string DoorType, std::string doorColour);
	std::string getDoorType();

	std::string getColour();
};

