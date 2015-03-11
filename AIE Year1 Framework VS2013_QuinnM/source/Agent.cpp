#include "Agent.h"

float const Agent::resistance = 0.06f;
char* const Agent::texture = "images/invaders/invaders_1_00.png";
bool Agent::drawVelocity = false;

unsigned int Agent::sprite = 0;
float const Agent::speedCap = 50;

Behaviour::Behaviour(Agent* in_owner) {
	owner = in_owner;
}
Behaviour::~Behaviour() { }

Agent::Agent(float in_x, float in_y) : Entity(in_x, in_y, 40, 40) {
	velocity.x = 0;
	velocity.y = 0;

	drag = true;

	maxVelocity = -1;

	behaviourPriority = std::vector<behaiviourArray>();

	if (sprite == 0) {
		sprite = CreateSprite(texture, 40, 40, true);
	}
}

Agent::~Agent() { } 

void Agent::SetSpeedCap(float in_speedCap) {
	maxVelocity = in_speedCap;
}

void Agent::AddPursue(Agent* in_target, float in_strength, int in_priority) {
	while (behaviourPriority.size() < in_priority + 1) {
		behaviourPriority.emplace_back(behaiviourArray());
	}
	if (maxVelocity != -1 && maxVelocity < speedCap) {
		behaviourPriority.at(in_priority).emplace_back(new PursueBehaviour(this, in_target, maxVelocity * in_strength));
	} else {
		behaviourPriority.at(in_priority).emplace_back(new PursueBehaviour(this, in_target, speedCap * in_strength));
	}
}

void Agent::AddEvade(Agent* in_target, float in_strength, int in_priority) {
	while (behaviourPriority.size() < in_priority + 1) {
		behaviourPriority.emplace_back(behaiviourArray());
	}
	if (maxVelocity != -1 && maxVelocity < speedCap) {
		behaviourPriority.at(in_priority).emplace_back(new EvadeBehaviour(this, in_target, maxVelocity * in_strength));
	} else {
		behaviourPriority.at(in_priority).emplace_back(new EvadeBehaviour(this, in_target, speedCap * in_strength));
	}
}

void Agent::AddWander(float in_circRadius, float in_jitter, float in_strength, float in_priority) {
	while (behaviourPriority.size() < in_priority + 1) {
		behaviourPriority.emplace_back(behaiviourArray());
	}
	if (maxVelocity != -1 && maxVelocity < speedCap) {
		behaviourPriority.at(in_priority).emplace_back(new WanderBehaviour(this, in_circRadius, in_jitter, maxVelocity * in_strength));
	}
	else {
		behaviourPriority.at(in_priority).emplace_back(new WanderBehaviour(this, in_circRadius, in_jitter, speedCap * in_strength));
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
	float speed;

	speed = std::sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));

	for (int i = 0; i < behaviourPriority.size(); i++) {
		for (int j = 0; j < behaviourPriority[i].size(); j++) {
			velocity += behaviourPriority[i][j]->GetForce();
		}

		speed = std::sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));

		if (speed > maxVelocity) {
			break;
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