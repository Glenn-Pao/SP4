#include "AIBase.h"

AI::AI()
{
}

AI::~AI()
{
}

AI::AI(CObjects::OBJECT_TYPE m_Object_Type, Vector3 pos, Vector3 scale, Mesh* mesh, int AIType)
{
	this->AIType = AIType;
	this->originalPos = pos;
	this->setPosition(pos);
	setObjType(m_Object_Type);
	setPosition(pos);
	setScale(scale);
	setMesh(mesh);
	this->wayPoints.push_back(this->originalPos);

	//Define the topleft and bottomright for the bounding box
	Vector3 topleft(this->getPosition().x - (getScale().x * 0.5), this->getPosition().y + (getScale().y * 0.5), 0);
	Vector3 bottomright(this->getPosition().x + (getScale().x * 0.5), this->getPosition().y - (getScale().y * 0.5), 0);

	//put it inside the bounding box (from object class)
	setBoundingBox(topleft, bottomright);
}

void AI::setAIType(int AIType)
{
	this->AIType = AIType;
}

int AI::getAIType()
{
	return AIType;
}

void AI::setOriginalPos(Vector3 originalPos)
{
	this->originalPos = originalPos;
}

Vector3 AI::getOriginalPos()
{
	return originalPos;
}

void AI::setFSM(CFSM::STATES state)
{
	fsm.state = state;
}

CFSM::STATES AI::getFSM()
{
	return fsm.state;
}

void AI::SetAIvariables4pt(Vector3 point1, Vector3 point2, Vector3 point3)
{
	wayPoints.push_back(Vector3(originalPos + point1));
	wayPoints.push_back(Vector3(originalPos + point1 + point2));
	wayPoints.push_back(Vector3(originalPos + point1 + point2 + point3));
	waypointsindex = 1;
	nextPoint = wayPoints[waypointsindex];
	timer = 0;
	timerCap = rand() % 15 + 10;
	speedInt = rand() % 5 + 1;
	if (speedInt == 1)
		speedFloat = 0.1f;
	else if (speedInt == 2)
		speedFloat = 0.2f;
	else if (speedInt == 3)
		speedFloat = 0.3f;
	else if (speedInt == 4)
		speedFloat = 0.4f;
	else if (speedInt == 5)
		speedFloat = 0.5f;
}

void AI::SetAIvariables5pt(Vector3 point1, Vector3 point2, Vector3 point3, Vector3 point4)
{
	wayPoints.push_back(Vector3(originalPos + point1));
	wayPoints.push_back(Vector3(originalPos + point1 + point2));
	wayPoints.push_back(Vector3(originalPos + point1 + point2 + point3));
	wayPoints.push_back(Vector3(originalPos + point1 + point2 + point3 + point4));
	
	waypointsindex = 1;
	nextPoint = wayPoints[waypointsindex];
	timer = 0;
	timerCap = rand() % 15 + 10;
	speedInt = rand() % 5 + 1;
	if (speedInt == 1)
		speedFloat = 0.1f;
	else if (speedInt == 2)
		speedFloat = 0.2f;
	else if (speedInt == 3)
		speedFloat = 0.3f;
	else if (speedInt == 4)
		speedFloat = 0.4f;
	else if (speedInt == 5)
		speedFloat = 0.5f;
}

void AI::UpdateFSM(double dt)
{
	//Define the topleft and bottomright for the bounding box
	Vector3 topleft(this->getPosition().x - (getScale().x * 0.5), this->getPosition().y + (getScale().y * 0.5), 0);
	Vector3 bottomright(this->getPosition().x + (getScale().x * 0.5), this->getPosition().y - (getScale().y * 0.5), 0);

	//put it inside the bounding box (from object class)
	setBoundingBox(topleft, bottomright);

	switch (fsm.state)
	{
	case CFSM::MOVING:
	{
		Vector3 direction = (nextPoint - getPosition()).Normalize();
		setPosition(getPosition() + direction + speedFloat);
		if ((nextPoint - getPosition()).Length() < 0.5)
		{
			fsm.state = CFSM::REACHED;
		}
	}
		break;
	case CFSM::REACHED:
	{
		waypointsindex = (waypointsindex + 1) % wayPoints.size();
		//waypointsindex = (wayPoints.size() - 1) <--- starting
		//waypointsindex = (waypointsindex - 1) % wayPoints.size();
		nextPoint = wayPoints[waypointsindex];
		fsm.state = CFSM::IDLE;
	}
		break;
	case CFSM::IDLE:
	{
		timer += 0.1f;
		if (timer > timerCap)
		{
			fsm.state = CFSM::MOVING;
			timer = 0;
		}
	}
		break;
	}
}