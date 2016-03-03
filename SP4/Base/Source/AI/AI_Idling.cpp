#include "AI_Idling.h"

#include "..\UsingLua.h"

CAI_Idling::CAI_Idling()
	: ai_type(STATIONARY)
	, fsm(IDLE)
	, idlingTimeLeft(0.0f)
	, vel(Vector3())
	, interactable(false)
	, interacted(false)
	, currentWaypointIndex(0)
	, targetWaypointIndex(0)
	, speed(0)
{
	//Read a value from the lua text file
	UseLuaFiles L;

	L.ReadFiles("Lua//ai_idling.lua");

	speed = L.DoLuaFloat("speed");
}

CAI_Idling::CAI_Idling(CObjects::OBJECT_TYPE m_Object_Type, Vector3 pos, Vector3 scale, Mesh* mesh, AI_TYPE ai_type)
	: ai_type(ai_type)
	, fsm(IDLE)
	, idlingTimeLeft(0.0f)
	, vel(Vector3())
	, interactable(false)
	, interacted(false)
	, currentWaypointIndex(0)
	, targetWaypointIndex(0)
	, speed(0)
{
	setPosition(pos);
	setObjType(m_Object_Type);
	setScale(scale);
	setMesh(mesh);

	//Define the topleft and bottomright for the bounding box
	Vector3 topleft(this->getPosition().x - (getScale().x * 0.5f), this->getPosition().y + (getScale().y * 0.5f), 0);
	Vector3 bottomright(this->getPosition().x + (getScale().x * 0.5f), this->getPosition().y - (getScale().y * 0.5f), 0);

	//put it inside the bounding box (from object class)
	setBoundingBox(topleft, bottomright);

	//Read a value from the lua text file
	UseLuaFiles L;

	L.ReadFiles("Lua//ai_idling.lua");

	speed = L.DoLuaFloat("speed");


	if (ai_type == MOVINGAROUND)
	{
		// Idling
		p_s.AddProbability(2);
		// MOVING
		p_s.AddProbability(1);
	}
}


CAI_Idling::~CAI_Idling()
{
}

void CAI_Idling::Update(double dt, CPlayerInfo* theHero, CMap* map)
{
	Vector3 prevPos = getPosition();
	switch (ai_type)
	{
		case MOVINGAROUND:
		{
			switch (fsm)
			{
				case IDLE:
				{
					// reduce idling time
					idlingTimeLeft -= (float)dt;

					// Check Time Left
					if (idlingTimeLeft <= 0.0f)
					{
						idlingTimeLeft = 0.0f;
						ChooseWhetherToIdling();
					}
				}
				break;
				case MOVING:
				{
					// moved AI
					setPosition(getPosition() + vel * (float)dt);

					// Check Distance
					if ((waypoints[currentWaypointIndex] - getPosition()).Length() >= (waypoints[currentWaypointIndex] - waypoints[targetWaypointIndex]).Length())
					{
						setPosition(waypoints[targetWaypointIndex]);
						vel.SetZero();
						currentWaypointIndex = targetWaypointIndex;
						ChooseWhetherToIdling();
					}
				}
				break;
			}
		}
		break;
		case GUARDIAN:
		{
			if (currentWaypointIndex != targetWaypointIndex)
			{
				// if no velocity
				if (vel.IsZero() == true)
				{
					vel = (waypoints[targetWaypointIndex] - waypoints[currentWaypointIndex]).Normalized() * speed;
				}

				// moved AI
				setPosition(getPosition() + vel * (float)dt);
				if (currentWaypointIndex == 0)
					theHero->setPosition(theHero->getPosition() + vel * (float)dt);

				// Check Distance
				if ((waypoints[currentWaypointIndex] - getPosition()).Length() >= (waypoints[currentWaypointIndex] - waypoints[targetWaypointIndex]).Length())
				{
					setPosition(waypoints[targetWaypointIndex]);
					vel.SetZero();
					currentWaypointIndex = targetWaypointIndex;

					// Move to next waypoint if not the last waypoint
					if (currentWaypointIndex != waypoints.size() - 1)
					{
						targetWaypointIndex++;
					}
				}
			}
		}
		break;
	}

	UpdateBoundingBox();
	if (getBoundingBox()->CheckCollision(*theHero->getBoundingBox()))
	{
		theHero->setPosition(theHero->getPosition() + getPosition() - prevPos);

		// If Hero Collide with anything
		if (theHero->CheckCollision(map) > 0)
		{
			// Push Hero back
			theHero->setPosition(theHero->getPosition() - getPosition() + prevPos);

			// Push AI back
			setPosition(prevPos);

			if (fsm == MOVING)
			{
				vel.SetZero();
				currentWaypointIndex = targetWaypointIndex;
				ChooseWhetherToIdling();
			}
		}
	}
}

// Waypoints
void CAI_Idling::AddWaypoint(Vector3 waypoint)
{
	waypoints.push_back(waypoint);
}
std::vector<Vector3> CAI_Idling::GetWaypoints()
{
	return waypoints;
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

// Current Waypoint Index
int CAI_Idling::GetCurrentWaypointIndex()
{
	return currentWaypointIndex;
}
void CAI_Idling::SetCurrentWaypointIndex(int currentWaypointIndex)
{
	this->currentWaypointIndex = currentWaypointIndex;
}

// Target Waypoint Index
int CAI_Idling::GetTargetWaypointIndex()
{
	return targetWaypointIndex;
}
void CAI_Idling::SetTargetWaypointIndex(int targetWaypointIndex)
{
	this->targetWaypointIndex = targetWaypointIndex;
}

// Update BoundingBox
void CAI_Idling::UpdateBoundingBox()
{
	//Define the topleft and bottomright for the bounding box
	Vector3 topleft(this->getPosition().x - (getScale().x * 0.5f), this->getPosition().y + (getScale().y * 0.5f), 0);
	Vector3 bottomright(this->getPosition().x + (getScale().x * 0.5f), this->getPosition().y - (getScale().y * 0.5f), 0);

	//put it inside the bounding box (from object class)
	setBoundingBox(topleft, bottomright);
}


// For MovingAround
void CAI_Idling::ChooseWhetherToIdling()
{
	fsm = (FSM)p_s.GetARandIntProbability();

	switch (fsm)
	{
		case IDLE:
		{
			// Set idling time
			idlingTimeLeft = Math::RandFloatMinMax(0.1f, 1.5f);
		}
		break;
		case MOVING:
		{
			// Set target to next waypoint if not the last waypoint
			if (currentWaypointIndex != waypoints.size() - 1)
			{
				targetWaypointIndex++;
			}
			else
			{
				targetWaypointIndex = 0;
			}

			// Set velocity
			if (vel.IsZero() == true)
			{
				vel = (waypoints[targetWaypointIndex] - waypoints[currentWaypointIndex]).Normalized() * speed;
			}
		}
		break;
	}
}