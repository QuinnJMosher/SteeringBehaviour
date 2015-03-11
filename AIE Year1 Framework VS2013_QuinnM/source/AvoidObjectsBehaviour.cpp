#include "AvoidObjectsBehaviour.h"

AvoidObjectsBehaviour::AvoidObjectsBehaviour(float in_avoidStrength, float in_maxSpeed, std::vector<Entity*>* pt_objects) {
	avoidStrength = in_avoidStrength;
	maxSpeed = in_maxSpeed;

	if (pt_objects == nullptr) {
		throw("Argument of AvoidObjectsBehaviour(float, std::vector<Entity*>*) cannot be null");
	}

	objects = pt_objects;
}

AvoidObjectsBehaviour::~AvoidObjectsBehaviour() { }

Point AvoidObjectsBehaviour::GetForce() {
	Point out = owner->GetVelocity();

	//obstacle avoidance dosn't quite work;
	//recalculate speed so we can make sure to keep it at the same level
	float speed = std::sqrt((owner->GetVelocity().x * owner->GetVelocity().x) + (owner->GetVelocity().y * owner->GetVelocity().y));
	//if i am moving
	if (!((out.x < 0.00001 && out.x > -0.00001) && //float eq for if (!(velocity.x == 0 && velocity.y == 0))
		(out.y < 0.00001 && out.y > -0.00001))) {
		//iterate throught a list of stuff to colide with (Loop)
		for (int i = 0; i < objects->size(); i++) {
			//don't attempt to colide with things if they are your target or yourself
			if (objects->at(i) != owner) {
				//check for colision allong current velocity
				if (objects->at(i)->RayCast(owner->position, owner->position + out * 3)) {
					//if collided then apply force away from here
					speed - avoidStrength;
				}
				//create new angle for next raycast
				float velocityAngle = atan2(out.x, -out.y);//get original angle
				Point editedDirection = Point(std::cos(velocityAngle + 0.2), -std::sin(velocityAngle + 0.2));//ad to angle, convert to vector
				editedDirection = (editedDirection * speed) * 3;//set velocity
				Point avoidForce;
				float avoidMag;
				//check colision at angle slightly above velocity;
				if (objects->at(i)->RayCast(owner->position, owner->position + editedDirection)) {
					avoidForce = objects->at(i)->position - owner->position;
					avoidMag = std::sqrt((avoidForce.x * avoidForce.x) + (avoidForce.y * avoidForce.y));
					avoidForce.x = avoidForce.x / avoidMag;
					avoidForce.y = avoidForce.y / avoidMag;
					avoidForce = avoidForce * -avoidStrength;//negitive to get the opposite direction
					out += avoidForce;
				}
				editedDirection = Point(std::cos(velocityAngle - 0.2), -std::sin(velocityAngle - 0.2));
				editedDirection = (editedDirection * speed) * 3;
				//check colision at angle slightly below velocity
				if (objects->at(i)->RayCast(owner->position, owner->position + editedDirection)) {
					//apply velocities as needed
					avoidForce = objects->at(i)->position - owner->position;
					avoidMag = std::sqrt((avoidForce.x * avoidForce.x) + (avoidForce.y * avoidForce.y));
					avoidForce.x = avoidForce.x / avoidMag;
					avoidForce.y = avoidForce.y / avoidMag;
					avoidForce = avoidForce * -avoidStrength;//negitive to get the opposite direction
					out += avoidForce;
				}
				//then ajust speed to be the same as it was before
				float changedSpeed = std::sqrt((owner->GetVelocity().x * owner->GetVelocity().x) + (owner->GetVelocity().y * owner->GetVelocity().y));
				out.x = (out.x / changedSpeed) * maxSpeed;
				out.y = (out.y / changedSpeed) * maxSpeed;
			}
		}
	}

	return out;
}