#ifndef _AvoidObjectsBehaviour_h_
#define _AvoidObjectsBehaviour_h_

#include "Behaviour.h"
#include <vector>

class AvoidObjectsBehaviour : public Behaviour {
public:
	AvoidObjectsBehaviour(float in_avoidStrength, float in_maxSpeed, std::vector<Entity*>* pt_objects);
	~AvoidObjectsBehaviour();

	virtual Point GetForce();

private:
	float avoidStrength;
	std::vector<Entity*>* objects;

	float maxSpeed;

};

#endif