#include "WanderBehaviour.h"

WanderBehaviour::WanderBehaviour(float in_radius, float in_jitter, float in_maxForce) {
	radius = in_radius;
	jitter = in_jitter;
	maxForce = in_maxForce;

	wanderPoint = 0;
}

WanderBehaviour::~WanderBehaviour() { }

Point WanderBehaviour::GetForce() {
	Point out;

	//find circle position (Where agent position is the origin) (velocity provides direction away unless it's 0,0)
	Point circlePos = owner->GetVelocity();

	//generate float from 0-2
	wanderPoint += ((((rand() % 2000) - 1000) / 10000.0f)) * jitter;
	if (wanderPoint < 0.0f) {
		wanderPoint *= -1;
	}
	while (wanderPoint > 2.0f)  {
		wanderPoint -= 2.0f;
	}

	//find position on circle at the generated number in radians
	Point RadPos;
	RadPos.x = circlePos.x + radius * std::cos(wanderPoint * std::_Pi);
	RadPos.y = circlePos.y + radius * std::sin(wanderPoint * std::_Pi);

	//normalize radpos
	float RadMag = std::sqrt((RadPos.x * RadPos.x) + (RadPos.y * RadPos.y));
	RadPos.x = RadPos.x / RadMag;
	RadPos.y = RadPos.y / RadMag;

	//set radpos's magnitude to the radius
	RadPos.x = RadPos.x * radius;
	RadPos.y = RadPos.y * radius;

	//add the new direction to our current velocity 
	out = RadPos;

	//compensate to keep velocity's magnitude the same
	float speed = std::sqrt((out.x * out.x) + (out.y * out.y));

	out.x = out.x / speed;
	out.y = out.y / speed;

	out.x = out.x * maxForce;
	out.y = out.y * maxForce;

	return out;
}