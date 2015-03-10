#include "Agent.h"

float const Agent::resistance = 0.06f;
char* const Agent::texture = "images/invaders/invaders_1_00.png";
bool Agent::drawVelocity = false;
std::vector<Entity*> Agent::objectList = std::vector<Entity*>();
float Agent::avoidStrength = 2;
unsigned int Agent::sprite = 0;
float const Agent::speedCap = 50;

Agent::Agent(float in_x, float in_y) : Entity(in_x, in_y, 40, 40) {
	velocity.x = 0;
	velocity.y = 0;

	drag = true;
	avoidObjects = false;

	personalCap = -1;
	activeBehavior = Behaviour::None;
	target = nullptr;

	wanderCircRadius = -1;
	wanderJitter = -1;
	wanderPoint = 1;

	if (sprite == 0) {
		sprite = CreateSprite(texture, 40, 40, true);
	}
}

Agent::~Agent() { } 

void Agent::ChangeBehaviour(Behaviour in_behavior) {
	activeBehavior = in_behavior;
}

void Agent::SetTarget(Agent* in_target) {
	target = in_target;
}

void Agent::SetSpeedCap(float in_speedCap) {
	personalCap = in_speedCap;
}

void Agent::AddForce(Point force) {
	velocity += force;
}

void Agent::SetForce(Point force) {
	velocity = force;
}

Point Agent::GetVelocity() {
	return velocity;
}

void Agent::SetWanderValues(float in_CircleRadius, float in_Jitter) {
	wanderCircRadius = in_CircleRadius;
	wanderJitter = in_Jitter;
}

void Agent::Update() {
	//find speed by calculating the magnitude of velocity
	float speed = std::sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));

	//Use Behaviour
	//switch vars
	Point circlePos;
	Point RadPos;
	float RadMag;
	switch (activeBehavior) {
	case Behaviour::Seek:
		if (target != nullptr) {
			//get target's position from the origin of position
			Point targetRelPos = target->position - position;
			float targetDist = std::sqrt((targetRelPos.x * targetRelPos.x) + (targetRelPos.y * targetRelPos.y));
			targetRelPos.x /= targetDist;
			targetRelPos.y /= targetDist;
			//add to velocity
			if (personalCap != -1 && personalCap < speedCap) {
				velocity.x = (targetRelPos.x * personalCap);
				velocity.y = (targetRelPos.y * personalCap);
			} else {
				velocity.x = (targetRelPos.x * speedCap);
				velocity.y = (targetRelPos.y * speedCap);
			}
		}
		break;
	case Behaviour::flee:
		if (target != nullptr) {
			//get target's position from the origin of position
			Point targetRelPos = target->position - position;
			float targetDist = std::sqrt((targetRelPos.x * targetRelPos.x) + (targetRelPos.y * targetRelPos.y));
			targetRelPos.x /= targetDist;
			targetRelPos.y /= targetDist;

			//reverse direction
			targetRelPos.x *= -1;
			targetRelPos.y *= -1;

			//add to velocity
			if (personalCap != -1 && personalCap < speedCap) {
				velocity.x = (targetRelPos.x * personalCap);
				velocity.y = (targetRelPos.y * personalCap);
			} else {
				velocity.x = (targetRelPos.x * speedCap);
				velocity.y = (targetRelPos.y * speedCap);
			}
		}
		break;
	case Behaviour::wander:
		//find circle position (Where agent position is the origin) (velocity provides direction away unless it's 0,0)
		circlePos = velocity;
		//generate float from 0-2
		wanderPoint += ((((rand() % 2000) - 1000) / 10000.0f)) * wanderJitter;
		if (wanderPoint < 0.0f) {
			wanderPoint *= -1;
		}
		while (wanderPoint > 2.0f)  {
			wanderPoint -= 2.0f;
		}
		//find position on circle at the generated number in radians
		RadPos;
		RadPos.x = circlePos.x + wanderCircRadius * std::cos(wanderPoint * std::_Pi);
		RadPos.y = circlePos.y + wanderCircRadius * std::sin(wanderPoint * std::_Pi);
		//normalize radpos
		RadMag = std::sqrt((RadPos.x * RadPos.x) + (RadPos.y * RadPos.y));
		RadPos.x = RadPos.x / RadMag;
		RadPos.y = RadPos.y / RadMag;
		//set radpos's magnitude to the radius
		RadPos.x = RadPos.x * wanderCircRadius;
		RadPos.y = RadPos.y * wanderCircRadius;
		//add the new direction to our current velocity 
		velocity += RadPos;
		//compensate to keep velocity's magnitude the same
		speed = std::sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));
		velocity.x = velocity.x / speed;
		velocity.y = velocity.y / speed;
		if (personalCap != -1 && personalCap < speedCap) {
			velocity.x = velocity.x * personalCap;
			velocity.y = velocity.y * personalCap;
		} else {
			velocity.x = velocity.x * speedCap;
			velocity.y = velocity.y * speedCap;
		}
		break;
	case Behaviour::pursue:
		if (target != nullptr) {
			//get target's position from the origin of position
			Point targetRelPos = (target->position + target->GetVelocity()) - position;
			float targetDist = std::sqrt((targetRelPos.x * targetRelPos.x) + (targetRelPos.y * targetRelPos.y));
			targetRelPos.x /= targetDist;
			targetRelPos.y /= targetDist;

			//add to velocity
			if (personalCap != -1 && personalCap < speedCap) {
				velocity.x = (targetRelPos.x * personalCap);
				velocity.y = (targetRelPos.y * personalCap);
			}
			else {
				velocity.x = (targetRelPos.x * speedCap);
				velocity.y = (targetRelPos.y * speedCap);
			}
		}
		break;
	case Behaviour::evade:
		if (target != nullptr) {
			//get target's position from the origin of position
			Point targetRelPos = (target->position + target->GetVelocity()) - position;
			float targetDist = std::sqrt((targetRelPos.x * targetRelPos.x) + (targetRelPos.y * targetRelPos.y));
			targetRelPos.x /= targetDist;
			targetRelPos.y /= targetDist;

			//reverse direction
			targetRelPos.x *= -1;
			targetRelPos.y *= -1;

			//add to velocity
			if (personalCap != -1 && personalCap < speedCap) {
				velocity.x = (targetRelPos.x * personalCap);
				velocity.y = (targetRelPos.y * personalCap);
			}
			else {
				velocity.x = (targetRelPos.x * speedCap);
				velocity.y = (targetRelPos.y * speedCap);
			}
		}
		break;
	case Behaviour::arrival:
		if (target != nullptr) {
			//get target's position from the origin of position
			Point targetRelPos = target->position - position;
			float targetDist = std::sqrt((targetRelPos.x * targetRelPos.x) + (targetRelPos.y * targetRelPos.y));
			targetRelPos.x /= targetDist;
			targetRelPos.y /= targetDist;
			//add to velocity
			if (personalCap != -1 && personalCap < speedCap) {
				if (personalCap < targetDist) {
					velocity.x = (targetRelPos.x * personalCap);
					velocity.y = (targetRelPos.y * personalCap);
				} else {
					velocity.x = (targetRelPos.x * targetDist);
					velocity.y = (targetRelPos.y * targetDist);
				}
			} else {
				if (speedCap < targetDist) {
					velocity.x = (targetRelPos.x * speedCap);
					velocity.y = (targetRelPos.y * speedCap);
				}
				else {
					velocity.x = (targetRelPos.x * targetDist);
					velocity.y = (targetRelPos.y * targetDist);
				}
			}
		}
		break;
	}

	//obstacle avoidance dosne quite work;
	//if obstacle Avoidance is turned on
	if (avoidObjects) {
		//recalculate speed so we can make sure to keep it at the same level
		speed = std::sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));

		//if i am moving
		if (!((velocity.x < 0.00001 && velocity.x > -0.00001) && //float eq for if (!(velocity.x == 0 && velocity.y == 0))
			(velocity.y < 0.00001 && velocity.y > -0.00001))) {

			//iterate throught a list of stuff to colide with (Loop)
			for (int i = 0; i < objectList.size(); i++) {

				//don't attempt to colide with things if they are your target or yourself
				if (objectList[i] != this && objectList[i] != target) {

					//check for colision allong current velocity
					if (objectList[i]->RayCast(position, position + velocity * 3)) {
						//if collided then apply force away from here
						speed - avoidStrength;
					}
					
					//create new angle for next raycast
					float velocityAngle = atan2(velocity.x, -velocity.y);//get original angle
					Point editedDirection = Point(std::cos(velocityAngle + 0.2), -std::sin(velocityAngle + 0.2));//ad to angle, convert to vector
					editedDirection = (editedDirection * speed) * 3;//set velocity

					Point avoidForce;
					float avoidMag;
					//check colision at angle slightly above velocity;
					if (objectList[i]->RayCast(position, position + editedDirection)) {
						avoidForce = objectList[i]->position - position;
						avoidMag = std::sqrt((avoidForce.x * avoidForce.x) + (avoidForce.y * avoidForce.y));
						avoidForce.x = avoidForce.x / avoidMag;
						avoidForce.y = avoidForce.y / avoidMag;
						avoidForce = avoidForce * -avoidStrength;//negitive to get the opposite direction
						velocity += avoidForce;
					}
					
					editedDirection = Point(std::cos(velocityAngle - 0.2), -std::sin(velocityAngle - 0.2));
					editedDirection = (editedDirection * speed) * 3;
					
					//check colision at angle slightly below velocity
					if (objectList[i]->RayCast(position, position + editedDirection)) {
						//apply velocities as needed
						avoidForce = objectList[i]->position - position;
						avoidMag = std::sqrt((avoidForce.x * avoidForce.x) + (avoidForce.y * avoidForce.y));
						avoidForce.x = avoidForce.x / avoidMag;
						avoidForce.y = avoidForce.y / avoidMag;
						avoidForce = avoidForce * -avoidStrength;//negitive to get the opposite direction
						velocity += avoidForce;
					}


					//then ajust speed to be the same as it was before
					float changedSpeed = std::sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));
					velocity.x = (velocity.x / changedSpeed) * speed;
					velocity.x = (velocity.x / changedSpeed) * speed;
				}
			}
		}
	}

	//cap speed
	//personal cap
	if (personalCap != -1 && personalCap <= speedCap) {
		if (speed > personalCap) {
			//get difference in speed
			float speedReduction = speed - personalCap;
			//get normal of x and y of velocity
			float normal_x = velocity.x / speed;
			float normal_y = velocity.y / speed;
			//reduece velociy.x by normal_x * speedRedduction
			velocity.x -= normal_x * speedReduction;
			velocity.y -= normal_y * speedReduction;
		}
	} else {
		//global cap
		if (speed > speedCap) {
			//get difference in speed
			float speedReduction = speed - personalCap;
			//get normal of x and y of velocity
			float normal_x = velocity.x / speed;
			float normal_y = velocity.y / speed;
			//reduece velociy.x by normal_x * speedRedduction
			velocity.x -= normal_x * speedReduction;
			velocity.y -= normal_y * speedReduction;
		}
	}

	//add drag
	if (drag) {
		if (velocity.x > 0) {
			if (velocity.x < resistance) {
				velocity.x = 0;
			} else {
				velocity.x -= resistance;
			}
		} else if (velocity.x < 0) {
			if (velocity.x > resistance) {
				velocity.x = 0;
			} else {
				velocity.x += resistance;
			}
		}

		if (velocity.y > 0) {
			if (velocity.y < resistance) {
				velocity.y = 0;
			} else {
				velocity.y -= resistance;
			}
		} else if (velocity.y < 0) {
			if (velocity.y > resistance) {
				velocity.y = 0;
			} else {
				velocity.y += resistance;
			}
		}
	}

	//loop screen
	if (position.x + (width / 2) > 900) {
		//position.x = (width / 2);
		velocity.x *= -1;
		position.x = 900 - width;
	} else if (position.x - (width / 2) < 0) {
		//position.x = 900 - (width / 2);
		velocity.x *= -1;
		position.x = 0 + width;
	}

	if (position.y + (height / 2) > 600) {
		//position.y = (height / 2);
		velocity.y *= -1;
		position.y = 600 - height;
	} else if (position.y - (height / 2) < 0) {
		//position.y = 600 - (height / 2);
		velocity.y *= -1;
		position.y = 0 + height;
	}

	//move Agent
	position += velocity;
}

void Agent::Draw() {
	MoveSprite(sprite, position.x, position.y);
	DrawSprite(sprite);
	if (drawVelocity) {
		DrawLine(position.x, position.y, position.x + (velocity.x * 10), position.y + (velocity.y * 10), SColour(255, 0, 0, 255));
	}
}

void Agent::ToggleDrag() {
	drag = !drag;
}

void Agent::ToggleAvoidObsticals() {
	avoidObjects = !avoidObjects;
}

void Agent::ToggleVelocityLine() {
	drawVelocity = !drawVelocity;
}

void Agent::AddObject(Entity* in_object) {
	objectList.emplace_back(in_object);
}

void Agent::RemoveObject(Entity* in_object) {
	for (int i = 0; i < objectList.size(); i++) {
		if (objectList[i] == in_object) {
			objectList.erase(objectList.begin() + i);
			break;
		}
	}
}