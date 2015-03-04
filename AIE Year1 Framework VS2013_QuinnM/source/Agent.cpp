#include "Agent.h"

float const Agent::resistance = 0.06f;
char* const Agent::texture = "images/crate_sideup.png";
unsigned int Agent::sprite = 0;

Agent::Agent(float in_x, float in_y) : Entity(in_x, in_y, 40, 40) {
	velocity.x = 0;
	velocity.y = 0;

	if (sprite == 0) {
		sprite = CreateSprite(texture, 40, 40, true);
	}
}

Agent::~Agent() { } 

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
	position += velocity;

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
}

void Agent::Draw() {
	MoveSprite(sprite, position.x, position.y);
	DrawSprite(sprite);
}