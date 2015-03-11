#include "PursueBehaviour.h"

PursueBehaviour::PursueBehaviour(Agent* in_owner, Agent* pt_target, float in_maxForce) : Behaviour(in_owner) {
	if (pt_target == nullptr) {
		throw("Argument of PursueBehaviour(Agent*) cannot be null");
	}

	target = pt_target;
	maxForce = in_maxForce;
}

PursueBehaviour::~PursueBehaviour() { }

Point PursueBehaviour::GetForce() {

	Point out_velocity = Point(0, 0);

	//get target's position from the origin of position
	Point targetDirectPos = (target->position) - owner->position;
	float targetDirectDist = std::sqrt((targetDirectPos.x * targetDirectPos.x) + (targetDirectPos.y * targetDirectPos.y));

	if (targetDirectDist > maxForce) {//if far away find where the target is going
		Point TargetRelPos = (target->position + target->GetVelocity()) - owner->position;
		float targetRelDist = std::sqrt((TargetRelPos.x * TargetRelPos.x) + (TargetRelPos.y * TargetRelPos.y));
		TargetRelPos.x /= targetRelDist;
		TargetRelPos.y /= targetRelDist;

		out_velocity.x = (TargetRelPos.x * maxForce);
		out_velocity.y = (TargetRelPos.y * maxForce);
		
	} else {//if they're close then go strait to them
		targetDirectPos.x /= targetDirectDist;
		targetDirectPos.y /= targetDirectDist;

		//add to velocity
		out_velocity.x = (targetDirectPos.x * (maxForce - targetDirectDist));
		out_velocity.y = (targetDirectPos.y * (maxForce - targetDirectDist));

	}

	return out_velocity;
}