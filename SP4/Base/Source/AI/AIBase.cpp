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

void AI::SetAIvariables(Vector3 point1, Vector3 point2, Vector3 point3)
{
	wayPoints.push_back(Vector3(originalPos + point1));
	wayPoints.push_back(Vector3(originalPos + point1 + point2));
	wayPoints.push_back(Vector3(originalPos + point1 + point2 + point3));
	waypointsindex = 1;
	nextPoint = wayPoints[waypointsindex];
	timer = 0;
}

void AI::UpdateFSM(double dt)
{
	switch (fsm.state)
	{
	case CFSM::MOVING:
	{
		Vector3 direction = (nextPoint - getPosition()).Normalize();
		setPosition(getPosition() + direction + 0.1f);
		if ((nextPoint - getPosition()).Length() < 0.5)
		{
			fsm.state = CFSM::REACHED;
		}
	}
		break;
	case CFSM::REACHED:
	{
		waypointsindex = (waypointsindex + 1) % 4;
		nextPoint = wayPoints[waypointsindex];
		fsm.state = CFSM::IDLE;
	}
		break;
	case CFSM::IDLE:
	{
		timer += 0.1f;
		if (timer > 10.f)
		{
			fsm.state = CFSM::MOVING;
			timer = 0;
		}
	}
		break;
	}
}