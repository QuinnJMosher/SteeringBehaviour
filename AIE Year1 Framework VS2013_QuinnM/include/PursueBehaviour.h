#ifndef _PursueBehaviour_h_
#define _PursueBehaviour_h_

#include "Agent.h"

class PursueBehaviour : public Behaviour {
public:
	PursueBehaviour(Agent* in_owner, Agent* pt_target, float in_maxForce);
	~PursueBehaviour();

	virtual Point GetForce();
private:
	Agent* target;
	float maxForce;
};

#endif