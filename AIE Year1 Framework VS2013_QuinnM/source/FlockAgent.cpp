#include "FlockAgent.h"

float const FlockAgent::resistance = 0.06f;
char* const FlockAgent::texture = "images/invaders/invaders_1_00.png";
bool FlockAgent::drawVelocity = false;
bool FlockAgent::drawNeighbourhood = false;

unsigned int FlockAgent::sprite = 0;
float const FlockAgent::speedCap = 50;

FlockAgent::FlockAgent(float in_x, float in_y) : Entity(in_x, in_y, 20, 20) {
	velocity.x = 0;
	velocity.y = 0;

	drag = true;

	frame = 0;

	maxVelocity = -1;

	//flocking stuff
	neighbourhood = std::vector<FlockAgent*>();
	neighbourhoodRadius = 0;
	world = nullptr;

	if (sprite == 0) {
		sprite = CreateSprite(texture, 20, 20, true);
	}
}

FlockAgent::~FlockAgent() { }

void FlockAgent::SetSpeedCap(float in_speedCap) {
	maxVelocity = in_speedCap;
}

void FlockAgent::AddForce(Point force) {
	velocity += force;
}

void FlockAgent::SetForce(Point force) {
	velocity = force;
}

Point FlockAgent::GetVelocity() {
	return velocity;
}

void FlockAgent::Update() {
	//frame++;
	float speed;

	speed = std::sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));

	//if (frame % 1 == 0) {
		UpdateNeighbourhood();
	//}

	//if (frame % 1 == 0) {
		Point flockingVelocity = Point(0, 0);

		flockingVelocity += Separation(maxVelocity * 0.45f);
		//flockingVelocity += Alignment(maxVelocity * 0.4f);
		flockingVelocity += Cohesion(maxVelocity * 0.3f);

		velocity += flockingVelocity;
	//}

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
	}
	else {
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
			}
			else {
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
	}
	else if (position.x - (width / 2) < 0) {
		//position.x = 900 - (width / 2);
		velocity.x *= -1;
		position.x = 0 + width;
	}

	if (position.y + (height / 2) > 600) {
		//position.y = (height / 2);
		velocity.y *= -1;
		position.y = 600 - height;
	}
	else if (position.y - (height / 2) < 0) {
		//position.y = 600 - (height / 2);
		velocity.y *= -1;
		position.y = 0 + height;
	}

	//move Agent
	position += velocity;
}

void FlockAgent::Draw() {
	MoveSprite(sprite, position.x, position.y);
	DrawSprite(sprite);
	if (drawVelocity) {
		DrawLine(position.x, position.y, position.x + (velocity.x * 10), position.y + (velocity.y * 10), SColour(255, 0, 0, 255));
	}
	if (drawNeighbourhood) {
		DrawLine(position.x, position.y - neighbourhoodRadius, position.x, position.y + neighbourhoodRadius, SColour(0, position.x, position.y, 255));
		DrawLine(position.x - neighbourhoodRadius, position.y, position.x + neighbourhoodRadius, position.y, SColour(0, position.x, position.y, 255));
	}
}

void FlockAgent::ToggleDrag() {
	drag = !drag;
}

void FlockAgent::ToggleVelocityLine() {
	drawVelocity = !drawVelocity;
}

void FlockAgent::ToggleNeighbourhoods() {
	drawNeighbourhood = !drawNeighbourhood;
}

void FlockAgent::SetWorld(std::vector<FlockAgent*>* pt_world) {
	world = pt_world;
}
void FlockAgent::SetNeighbourhood(float in_radius) {
	neighbourhoodRadius = in_radius;
}

Point FlockAgent::Separation(float in_repulsion) {

	if (neighbourhood.size() == 0) {
		return Point(0, 0);
	}

	Point totalRepulsion = Point(0, 0);

	for (int i = 0; i < neighbourhood.size(); i++) {
		Point currentRepulsion = (neighbourhood[i]->position  - position) * -1;
		float repulseMag = std::sqrt(std::pow(currentRepulsion.x, 2) + std::pow(currentRepulsion.y, 2));

		if (repulseMag < neighbourhoodRadius / 2) {
			totalRepulsion += currentRepulsion;
		}
	}

	float magnetude = std::sqrt(std::pow(totalRepulsion.x, 2) + std::pow(totalRepulsion.y, 2));

	if (magnetude < 0.0000001 && magnetude > -0.0000001) {//float EQ for magnetude == 0;
		return Point(0, 0);
	}

	totalRepulsion.x /= magnetude;
	totalRepulsion.y /= magnetude;

	totalRepulsion.x *= in_repulsion;
	totalRepulsion.y *= in_repulsion;

	return totalRepulsion;
}
Point FlockAgent::Alignment(float in_pull) {
	return Point(0, 0);
}
Point FlockAgent::Cohesion(float in_attraction) {
	if (neighbourhood.size() == 0) {
		return Point(0, 0);
	}

	Point averagePos = Point(0, 0);
	int numItems = 0;
	
	for (int i = 0; i < neighbourhood.size(); i++) {
		Point currentDist = neighbourhood[i]->position - position;
		float posDist = std::sqrt(std::pow(currentDist.x, 2) + std::pow(currentDist.y, 2));

		if (posDist > neighbourhoodRadius / 2) {
			averagePos += neighbourhood[i]->position;
			numItems++;
		}
	}

	averagePos.x /= numItems;
	averagePos.y /= numItems;

	averagePos -= position;

	float magnitude = std::sqrt(std::pow(averagePos.x, 2) + std::pow(averagePos.y, 2));

	averagePos.x /= magnitude;
	averagePos.y /= magnitude;

	averagePos.x *= in_attraction;
	averagePos.y *= in_attraction;

	return averagePos;
}

void FlockAgent::UpdateNeighbourhood() {
	//clear neighbourhood
	neighbourhood.clear();
	//if world is null or radius is 0 then return
	if (world != nullptr && neighbourhoodRadius != 0) {
		//loop through world;
		for (int i = 0; i < world->size(); i++) {
			//if distance from current Agent is less than radius add to neighbourhood
			Point positionDiference = world->at(i)->position - position;
			float distance = std::sqrt((positionDiference.x * positionDiference.x) + (positionDiference.y * positionDiference.y));

			if (distance < neighbourhoodRadius && world->at(i) != this) {
				neighbourhood.emplace_back(world->at(i));
			}

		}
	}
}