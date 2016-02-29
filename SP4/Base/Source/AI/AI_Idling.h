#pragma once

#include <vector>
#include <Vector3.h>
#include "..\..\Source\GameMechanics\Objects\Objects.h"
#include "..\PlayerInfo.h"
#include "FSM.h"

class CAI_Idling : public CObjects
{
	enum FSM
	{
		IDLE,
		MOVING,
		NUM_OF_FSM
	};
public:
	enum AI_TYPE
	{
		GUARDIAN,
		STATIONARY,
		MOVABLE,
		NUM_OF_AI_TYPE
	};
	CAI_Idling();
	CAI_Idling(CObjects::OBJECT_TYPE m_Object_Type, Vector3 pos, Vector3 scale, Mesh* mesh, AI_TYPE ai_type);
	~CAI_Idling();

	void Update(double dt, CPlayerInfo* theHero);

	// Waypoints
	void AddWaypoint(Vector3 waypoint);
	// Get AI Types
	AI_TYPE GetAI_Type();
	// Interactable
	bool IfInteractable();
	void SetIfInteractable(bool interactable);
	// Interacted
	bool IfInteracted();
	void SetIfInteracted(bool interacted);

private:
	std::vector<Vector3> waypoints;
	AI_TYPE ai_type;
	FSM fsm;
	float idlingTimeLeft;
	Vector3 vel;
	bool interactable;
	bool interacted;
};