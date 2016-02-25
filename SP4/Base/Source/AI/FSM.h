#pragma once


#include <Vector3.h>
#include <vector>

class CFSM
{
public:
	CFSM();
	~CFSM();
	enum STATES
	{
		NOSTATE,
		IDLE,
		MOVING,
		REACHED,
		NUM_STATES,
	};
	/*void setState(CFSM::STATES);
	CFSM::STATES getState();*/
	
	CFSM::STATES state;
private:
	
};