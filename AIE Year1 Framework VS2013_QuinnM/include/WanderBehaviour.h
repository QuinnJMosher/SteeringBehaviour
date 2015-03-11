#ifndef _WanderBehaviour_h_
#define _WanderBehaviour_h_

#include "Behaviour.h"

class WanderBehaviour : public Behaviour {
public:
	WanderBehaviour(float in_radius, float in_jitter, float in_maxForce);
	~WanderBehaviour();

	virtual Point GetForce();

private:
	float radius;
	float jitter;
	float maxForce;

	float wanderPoint;
};

#endif