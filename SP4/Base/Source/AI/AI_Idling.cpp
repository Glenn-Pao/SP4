#include "AI_Idling.h"

CAI_Idling::CAI_Idling()
	: ai_type(STATIONARY)
	, fsm(IDLE)
	, idlingTimeLeft(0.0f)
	, vel(Vector3())
	, interactable(false)
	, interacted(false)
{
}

CAI_Idling::CAI_Idling(CObjects::OBJECT_TYPE m_Object_Type, Vector3 pos, Vector3 scale, Mesh* mesh, AI_TYPE ai_type)
	: ai_type(ai_type)
	, fsm(IDLE)
	, idlingTimeLeft(0.0f)
	, vel(Vector3())
	, interactable(false)
	, interacted(false)
{
	setPosition(pos);
	setObjType(m_Object_Type);
	setScale(scale);
	setMesh(mesh);

	//Define the topleft and bottomright for the bounding box
	Vector3 topleft(this->getPosition().x - (getScale().x * 0.5), this->getPosition().y + (getScale().y * 0.5), 0);
	Vector3 bottomright(this->getPosition().x + (getScale().x * 0.5), this->getPosition().y - (getScale().y * 0.5), 0);

	//put it inside the bounding box (from object class)
	setBoundingBox(topleft, bottomright);
}


CAI_Idling::~CAI_Idling()
{
}

void CAI_Idling::Update(double dt, CPlayerInfo* theHero)
{
	//Define the topleft and bottomright for the bounding box
	Vector3 topleft(this->getPosition().x - (getScale().x * 0.5), this->getPosition().y + (getScale().y * 0.5), 0);
	Vector3 bottomright(this->getPosition().x + (getScale().x * 0.5), this->getPosition().y - (getScale().y * 0.5), 0);

	//put it inside the bounding box (from object class)
	setBoundingBox(topleft, bottomright);

	//switch (fsm.state)
	//{
	//case CFSM::MOVING:
	//{
	//	Vector3 direction = (nextPoint - getPosition()).Normalize();
	//	setPosition(getPosition() + direction + speedFloat);
	//	if ((nextPoint - getPosition()).Length() < 0.5)
	//	{
	//		fsm.state = CFSM::REACHED;
	//	}
	//}
	//	break;
	//case CFSM::REACHED:
	//{
	//	waypointsindex = (waypointsindex + 1) % wayPoints.size();
	//	//waypointsindex = (wayPoints.size() - 1) <--- starting
	//	//waypointsindex = (waypointsindex - 1) % wayPoints.size();
	//	nextPoint = wayPoints[waypointsindex];
	//	fsm.state = CFSM::IDLE;
	//}
	//	break;
	//case CFSM::IDLE:
	//{
	//	timer += 0.1f;
	//	if (timer > timerCap)
	//	{
	//		fsm.state = CFSM::MOVING;
	//		timer = 0;
	//	}
	//}
	//	break;
	//}
}

// Waypoints
void CAI_Idling::AddWaypoint(Vector3 waypoint)
{
	waypoints.push_back(waypoint);
}

// Get AI Types
CAI_Idling::AI_TYPE CAI_Idling::GetAI_Type()
{
	return ai_type;
}

// Interactable
bool CAI_Idling::IfInteractable()
{
	return interactable;
}
void CAI_Idling::SetIfInteractable(bool interactable)
{
	this->interactable = interactable;
}

// Interacted
bool CAI_Idling::IfInteracted()
{
	return interacted;
}
void CAI_Idling::SetIfInteracted(bool interacted)
{
	this->interacted = interacted;
}