#ifndef _EvadeBehaviour_h_
#define _EvadeBehaviour_h_

#include "Behaviour.h"

class EvadeBehaviour : public Behaviour {
public:
	EvadeBehaviour(Agent* pt_target, float in_maxForce);
	~EvadeBehaviour();

	virtual Point GetForce();
private:
	Agent* target;
	float maxForce;
};

#endif