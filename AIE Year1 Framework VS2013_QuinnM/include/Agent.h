#ifndef _Agent_h_
#define _Agent_h_

#include "Entity.h" //parent
#include "AIE.h"
#include <random>
#include <ctime>
#include <vector>

enum Behaviour
{
	None,
	Seek,
	flee,
	wander,
	pursue,
	evade,
	arrival
};

class Agent : public Entity {
public:
	Agent(float in_x, float in_y);
	~Agent();

	void ChangeBehaviour(Behaviour in_behavior);
	void SetTarget(Agent* in_target);
	void SetSpeedCap(float in_speedCap);

	void AddForce(Point force);
	void SetForce(Point force);

	Point GetVelocity();

	void SetWanderValues(float in_CircleRadius, float in_Jitter);

	virtual void Update();
	virtual void Draw();

	void ToggleDrag();
	void ToggleAvoidObsticals();
	static void ToggleVelocityLine();

	static void AddObject(Entity* in_object);
	static void RemoveObject(Entity* in_object);

private:
	Point velocity;
	bool drag;

	float personalCap;

	Behaviour activeBehavior;
	Agent* target;

	float wanderCircRadius;
	float wanderJitter;
	float wanderPoint;

	bool avoidObjects;
	static float avoidStrength;
	static std::vector<Entity*> objectList;

	static bool drawVelocity;

	static float const resistance;
	static float const speedCap;

	static char* const texture;
	static unsigned int sprite;
};

#endif