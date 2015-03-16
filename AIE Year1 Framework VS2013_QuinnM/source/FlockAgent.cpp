#include "FlockAgent.h"

float const FlockAgent::resistance = 0.06f;
char* const FlockAgent::texture = "images/invaders/invaders_1_00.png";
bool FlockAgent::drawVelocity = false;

unsigned int FlockAgent::sprite = 0;
float const FlockAgent::speedCap = 50;

FlockAgent::FlockAgent(float in_x, float in_y) : Entity(in_x, in_y, 40, 40) {
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
		sprite = CreateSprite(texture, 40, 40, true);
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
	frame++;
	float speed;

	speed = std::sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));

	if (frame % 10 == 0) {
		UpdateNeighbourhood();
	}

	if (frame % 10 == 0) {
		Point flockingVelocity = Point(0, 0);

		flockingVelocity += Separation(maxVelocity * 0.3f);
		flockingVelocity += Cohesion(maxVelocity * 0.3f);

		velocity = flockingVelocity;
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
}

void FlockAgent::ToggleDrag() {
	drag = !drag;
}

void FlockAgent::ToggleVelocityLine() {
	drawVelocity = !drawVelocity;
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
	Point separationVel = Point();
	int count = 0;

	//add all directions into one large force
	for (int i = 0; i < neighbourhood.size(); i++) {
		//find the current force
		Point currentForce = neighbourhood[i]->position - position;
		float magnitude = std::sqrt((currentForce.x * currentForce.x) + (currentForce.y * currentForce.y));

		if (magnitude < neighbourhoodRadius / 2) {

			//if it's further away than our given power then set the force's magnitude to the given maximum
			currentForce.x /= magnitude;
			currentForce.y /= magnitude;

			currentForce.x * in_repulsion;
			currentForce.y * in_repulsion;

			separationVel -= currentForce;
			count++;
		}
	}

	//find the mean force
	separationVel.x /= count;
	separationVel.y /= count;

	return separationVel;
}
Point FlockAgent::Alignment() {
	return Point(0, 0);
}
Point FlockAgent::Cohesion(float in_attraction) {
	if (neighbourhood.size() == 0) {
		return Point(0, 0);
	}

	//if (neighbourhood.size() == 1) {
	//	return Point(0, 0);
	//}

	Point neighborAvg = Point();
	float numPos = 0;

	//add up all positions
	for (int i = 0; i < neighbourhood.size(); i++) {
		Point otherDif = neighbourhood[i]->position - position;
		float otherDist = std::sqrt((otherDif.x * otherDif.x) + (otherDif.y * otherDif.y));
		if (otherDist > neighbourhoodRadius / 2) {
			neighborAvg += neighbourhood[i]->position;
			numPos++;
		}
	}

	//average the position
	neighborAvg.x /= numPos;
	neighborAvg.y /= numPos;

	//make position relitive to our position
	neighborAvg -= position;
	//normalize relitive position
	float magnitude = std::sqrt((neighborAvg.x * neighborAvg.x) + (neighborAvg.y * neighborAvg.y));

	neighborAvg.x /= magnitude;
	neighborAvg.y /= magnitude;
	//set the relitive positions magnitude to our given attraction value
	neighborAvg.x *= in_attraction;
	neighborAvg.y *= in_attraction;

	magnitude = std::sqrt((neighborAvg.x * neighborAvg.x) + (neighborAvg.y * neighborAvg.y));

	return neighborAvg;
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