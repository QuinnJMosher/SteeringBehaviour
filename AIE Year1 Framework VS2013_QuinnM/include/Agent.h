#ifndef _Agent_h_
#define _Agent_h_

#include "Entity.h" //parent
#include "AIE.h"
#include <random>
#include <ctime>
#include <vector>

class Agent;

enum BehaviourType {
	Pursue,
	Evade,
	Wander,
	Flock
};

struct Behaviour {
	Behaviour(BehaviourType in_type, Agent* in_target, float  in_strength);
	BehaviourType type;
	Agent* target;
	float strength;//a percent value
};

class Agent : public Entity {
public:
	Agent(float in_x, float in_y);
	~Agent();

	void SetSpeedCap(float in_speedCap);

	void AddPursue(Agent* in_target, float in_strength);
	void AddEvade(Agent* in_target, float in_strength);
	void AddWander(float in_circDist, float in_circRadius, float in_jitter, float in_strength);

	void RemovePursue(Agent* in_target);
	void RemoveEvade(Agent* in_target);
	void RemoveWander();

	void AddForce(Point force);
	void SetForce(Point force);

	Point GetVelocity();

	virtual void Update();
	virtual void Draw();

	void ToggleDrag();
	static void ToggleVelocityLine();

private:
	Point GetPersue(Agent* in_target, float in_speed);
	Point GetEvade(Agent* in_target, float in_speed);
	Point GetWander(float in_speed);

	Point velocity;
	bool drag;

	int frame;

	float maxVelocity;

	std::vector<Behaviour> behaiviourArray;

	//wander stuff;
	float wanderPoint;
	float circDist;
	float circRadius;
	float jitter;

	static bool drawVelocity;

	static float const resistance;
	static float const speedCap;

	static char* const texture;
	static unsigned int sprite;
};

#endif