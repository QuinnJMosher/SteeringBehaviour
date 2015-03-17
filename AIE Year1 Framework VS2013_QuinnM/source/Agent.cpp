#include "Agent.h"

float const Agent::resistance = 0.06f;
char* const Agent::texture = "images/invaders/invaders_1_00.png";
bool Agent::drawVelocity = false;

unsigned int Agent::sprite = 0;
float const Agent::speedCap = 50;

Behaviour::Behaviour(BehaviourType in_type, Agent* in_target, float  in_strength) {
	type = in_type;
	target = in_target;
	strength = in_strength;
}

Agent::Agent(float in_x, float in_y) : Entity(in_x, in_y, 40, 40) {
	velocity.x = 0;
	velocity.y = 0;

	drag = true;

	frame = 0;

	maxVelocity = -1;

	behaiviourArray = std::vector<Behaviour>();

	if (sprite == 0) {
		sprite = CreateSprite(texture, 40, 40, true);
	}
}

Agent::~Agent() { } 

void Agent::SetSpeedCap(float in_speedCap) {
	maxVelocity = in_speedCap;
}

void Agent::AddPursue(Agent* in_target, float in_strength) {
	behaiviourArray.emplace_back(Behaviour(Pursue, in_target, in_strength));
}

void Agent::AddEvade(Agent* in_target, float in_strength) {
	behaiviourArray.emplace_back(Behaviour(Evade, in_target, in_strength));
}

void Agent::AddWander(float in_circDist, float in_circRadius, float in_jitter, float in_strength) {
	circDist = in_circDist;
	circRadius = in_circRadius;
	jitter = in_jitter;
	wanderPoint = 1;
	for (int i = 0; i < behaiviourArray.size(); i++) {
		if (behaiviourArray[i].type == Wander) {
			behaiviourArray[i].strength = in_strength;
			return;
		}
	}

	behaiviourArray.emplace_back(Behaviour(Wander, nullptr, in_strength));
}

void Agent::RemovePursue(Agent* in_target) {
	for (int i = 0; i < behaiviourArray.size(); i++) {
		if (behaiviourArray[i].target == in_target && behaiviourArray[i].type == Pursue) {
			behaiviourArray.erase(behaiviourArray.begin() + i);
			break;
		}
	}
}

void Agent::RemoveEvade(Agent* in_target) {
	for (int i = 0; i < behaiviourArray.size(); i++) {
		if (behaiviourArray[i].target == in_target && behaiviourArray[i].type == Evade) {
			behaiviourArray.erase(behaiviourArray.begin() + i);
			break;
		}
	}
}

void Agent::RemoveWander() {
	for (int i = 0; i < behaiviourArray.size(); i++) {
		if (behaiviourArray[i].type == Wander) {
			behaiviourArray.erase(behaiviourArray.begin() + i);
			break;
		}
	}
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
	frame++;
	float speed;

	speed = std::sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));

	if (frame % 7 == 0) {
		for (int i = 0; i < behaiviourArray.size(); i++) {
			switch (behaiviourArray[i].type) {
			case Pursue:
				velocity += GetPersue(behaiviourArray[i].target, maxVelocity * behaiviourArray[i].strength);
				break;
			case Evade:
				velocity += GetEvade(behaiviourArray[i].target, maxVelocity * behaiviourArray[i].strength);
				break;
			case Wander:
				velocity += GetWander(maxVelocity * behaiviourArray[i].strength);
			}

			speed = std::sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));

			if (speed > maxVelocity) {
				break;
			}

		}
	}

	//cap speed
	//personal cap
	if (maxVelocity != -1 && maxVelocity <= speedCap) {
		if (speed > maxVelocity) {
			//get difference in speed
			float speedReduction = speed - maxVelocity;
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
			float speedReduction = speed - maxVelocity;
			//get normal of x and y of velocity
			float normal_x = velocity.x / speed;
			float normal_y = velocity.y / speed;
			//reduece velociy.x by normal_x * speedRedduction
			velocity.x -= normal_x * speedReduction;
			velocity.y -= normal_y * speedReduction;
		}
	}

	speed = std::sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));

	//add drag
	if (drag) {
		if (!(velocity.x < 0.00001 && velocity.x > -0.00001) || //float eq for if (velocity.x != 0 || velocity.y != 0)
			!(velocity.y < 0.00001 && velocity.y > -0.00001)) {
			float normal_x = velocity.x / speed;
			float normal_y = velocity.y / speed;

			if (speed > resistance) {
				velocity.x = normal_x * (speed - resistance);
				velocity.y = normal_y * (speed - resistance);
			} else {
				velocity.x = 0;
				velocity.y = 0;
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

void Agent::ToggleVelocityLine() {
	drawVelocity = !drawVelocity;
}

Point Agent::GetPersue(Agent* in_target, float in_speed) {
	Point out_velocity = Point(0, 0);

	//get target's position from the origin of position
	Point targetDirectPos = (in_target->position) - position;
	float targetDirectDist = std::sqrt((targetDirectPos.x * targetDirectPos.x) + (targetDirectPos.y * targetDirectPos.y));

	if (targetDirectDist > in_speed) {//if far away find where the target is going
		Point TargetRelPos = (in_target->position + in_target->GetVelocity()) - position;
		float targetRelDist = std::sqrt((TargetRelPos.x * TargetRelPos.x) + (TargetRelPos.y * TargetRelPos.y));
		TargetRelPos.x /= targetRelDist;
		TargetRelPos.y /= targetRelDist;

		out_velocity.x = (TargetRelPos.x * in_speed);
		out_velocity.y = (TargetRelPos.y * in_speed);

	}
	else {//if they're close then go strait to them
		targetDirectPos.x /= targetDirectDist;
		targetDirectPos.y /= targetDirectDist;

		//add to velocity
		out_velocity.x = (targetDirectPos.x * (in_speed - targetDirectDist));
		out_velocity.y = (targetDirectPos.y * (in_speed - targetDirectDist));

	}

	return out_velocity;
}

Point Agent::GetEvade(Agent* in_target, float in_speed) {
	Point out_velocity = Point(0, 0);

	//get target's position from the origin of position
	Point targetDirectPos = (in_target->position) - position;
	float targetDirectDist = std::sqrt((targetDirectPos.x * targetDirectPos.x) + (targetDirectPos.y * targetDirectPos.y));

	if (targetDirectDist > in_speed) {//if far away find where the target is going
		Point TargetRelPos = (in_target->position + in_target->GetVelocity()) - position;
		float targetRelDist = std::sqrt((TargetRelPos.x * TargetRelPos.x) + (TargetRelPos.y * TargetRelPos.y));
		TargetRelPos.x /= targetRelDist;
		TargetRelPos.y /= targetRelDist;

		out_velocity.x = (TargetRelPos.x * (in_speed * -1));
		out_velocity.y = (TargetRelPos.y * (in_speed * -1));

	}
	else {//if they're close then go strait to them
		targetDirectPos.x /= targetDirectDist;
		targetDirectPos.y /= targetDirectDist;

		//add to velocity
		out_velocity.x = (targetDirectPos.x * ((in_speed - targetDirectDist) * -1));
		out_velocity.y = (targetDirectPos.y * ((in_speed - targetDirectDist) * -1));

	}

	return out_velocity;
}

Point Agent::GetWander(float in_speed) {
	Point out;

	//find circle position (Where agent position is the origin)
	Point currentDir = velocity;
	float currentSpeed = std::sqrt((currentDir.x * currentDir.x) + (currentDir.y * currentDir.y));
	if (currentSpeed > 0.0000001 && currentSpeed < 0.0000001) { //float eq for currentSpeed != 0
		currentDir.x /= currentSpeed;
		currentDir.y /= currentSpeed;
	}

	Point circlePos = currentDir * circDist;

	//generate float from 0-2
	wanderPoint += ((((rand() % 200) - 100) / 1000.0f)) * jitter;
	if (wanderPoint < 0.0f) {
		wanderPoint *= -1;
	}
	while (wanderPoint > 2.0f)  {
		wanderPoint -= 2.0f;
	}

	//find position on circle at the generated number in radians
	Point RadPos;
	RadPos.x = circlePos.x + circRadius * std::cos(wanderPoint * std::_Pi);
	RadPos.y = circlePos.y + circRadius * std::sin(wanderPoint * std::_Pi);

	//normalize radpos
	float RadMag = std::sqrt((RadPos.x * RadPos.x) + (RadPos.y * RadPos.y));
	RadPos.x = RadPos.x / RadMag;
	RadPos.y = RadPos.y / RadMag;

	//set radpos's magnitude to the radius
	RadPos.x = RadPos.x * circRadius;
	RadPos.y = RadPos.y * circRadius;

	//add the new direction to our current velocity 
	out = RadPos;

	//compensate to keep velocity's magnitude the same
	float speed = std::sqrt((out.x * out.x) + (out.y * out.y));

	out.x = out.x / speed;
	out.y = out.y / speed;

	out.x = out.x * in_speed;
	out.y = out.y * in_speed;

	return out;
}