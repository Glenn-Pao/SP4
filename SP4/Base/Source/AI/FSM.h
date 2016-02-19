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

		NUM_STATES,
	};
	void setState(CFSM::STATES);
	CFSM::STATES getState();

private:
	CFSM::STATES state;
};