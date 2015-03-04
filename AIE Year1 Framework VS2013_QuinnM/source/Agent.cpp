#include "Agent.h"

float const Agent::resistance = 0.06f;
char* const Agent::texture = "images/crate_sideup.png";
bool Agent::drawVelocity = false;

unsigned int Agent::sprite = 0;
float const Agent::speedCap = 50;

Agent::Agent(float in_x, float in_y) : Entity(in_x, in_y, 40, 40) {
	velocity.x = 0;
	velocity.y = 0;

	drag = true;

	personalCap = -1;
	activeBehavior = Behaviour::None;
	target = nullptr;

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

void Agent::Update() {
	//find speed by calculating the magnitude of velocity
	float speed = std::sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));
	//Use Behaviour
	switch (activeBehavior) {
	case Behaviour::Seek:
		if (target != nullptr) {
			//get target's position from the origin of position
			Point targetRelPos = target->position - position;
			float targetDist = std::sqrt((target->position.x * target->position.x) + (target->position.y * target->position.y));
			targetRelPos.x /= targetDist;
			targetRelPos.y /= targetDist;
			//add to velocity
			velocity.x += targetRelPos.x * 5;
			velocity.y += targetRelPos.y * 5;
		}
		break;
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
		position.x = (width / 2);
	} else if (position.x - (width / 2) < 0) {
		position.x = 900 - (width / 2);
	}

	if (position.y + (height / 2) > 600) {
		position.y = (height / 2);
	} else if (position.y - (height / 2) < 0) {
		position.y = 600 - (height / 2);
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

void Agent::ToggleVelocityLine() {
	drawVelocity = !drawVelocity;
}