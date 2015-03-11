#ifndef _Agent_h_
#define _Agent_h_

#include "Entity.h" //parent
#include "AIE.h"
#include <random>
#include <ctime>
#include <vector>

class Agent;

class Behaviour {
public:
	Behaviour(Agent* in_owner);
	~Behaviour();

	virtual Point GetForce() = 0;

protected:
	Agent* owner;
};

typedef std::vector<Behaviour*> behaiviourArray;

class Agent : public Entity {
public:
	Agent(float in_x, float in_y);
	~Agent();

	void SetSpeedCap(float in_speedCap);

	void AddPursue(Agent* in_target, float in_strength, int in_priority);
	void AddEvade(Agent* in_target, float in_strength, int in_priority);
	void AddWander(float in_circRadius, float in_jitter, float in_strength, float in_priority);

	void AddForce(Point force);
	void SetForce(Point force);

	Point GetVelocity();

	virtual void Update();
	virtual void Draw();

	void ToggleDrag();
	static void ToggleVelocityLine();

private:
	Point velocity;
	bool drag;

	float maxVelocity;

	std::vector<behaiviourArray> behaviourPriority;

	static bool drawVelocity;

	static float const resistance;
	static float const speedCap;

	static char* const texture;
	static unsigned int sprite;
};

//cpp only headers (avoids circular dependency)
#include "PursueBehaviour.h"
#include "EvadeBehaviour.h"
#include "WanderBehaviour.h"

#endif