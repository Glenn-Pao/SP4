#pragma once

#include <vector>
#include <Vector3.h>
#include "..\..\Source\GameMechanics\Objects\Objects.h"
#include "FSM.h"

class AI : public CObjects
{
public:
	AI();
	AI(CObjects::OBJECT_TYPE m_Object_Type, Vector3 pos, Vector3 scale, Mesh* mesh, int AIType);
	~AI();

	void setAIType(int);
	int getAIType();

	void setOriginalPos(Vector3);
	Vector3 getOriginalPos();

	void setFSM(CFSM::STATES);
	CFSM::STATES getFSM();


	void SetAIvariables4pt(Vector3 point1, Vector3 point2, Vector3 point3);
	void SetAIvariables5pt(Vector3 point1, Vector3 point2, Vector3 point3, Vector3 point4);

	void UpdateFSM(double dt);

private:
	Vector3 originalPos;
	Vector3 nextPoint;
	std::vector<Vector3> wayPoints;
	int waypointsindex;
	int AIType;
	CFSM fsm;
	float timer;
	
};