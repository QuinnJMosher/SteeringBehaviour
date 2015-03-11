#ifndef _Behaviour_h_
#define _Behaviour_h_

#include "Point.h"
#include "Agent.h"

class Behaviour {
public:
	Behaviour();
	~Behaviour();

	virtual Point GetForce() = 0;

protected:
	Agent* owner;
};

#endif